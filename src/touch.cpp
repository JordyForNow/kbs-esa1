#include "touch.h"

#include "defines.h"
#include "game.h"
#include "logger.h"
#include "score.h"
#include "render.h"

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

menu_t *menu_main = NULL;
menu_t *menu_play = NULL;
menu_t *menu_select_level = NULL;
menu_t *menu_score = NULL;
menu_t *menu_win = NULL;
menu_t *menu_lose = NULL;
menu_t *menu_waiting = NULL;

component_t *button_new(const char *text, menu_t *target, button_mode_t mode) {
    component_t *button = (component_t *)malloc(sizeof(component_t));
    if (!button)
        return NULL;

    button->text = strdup(text);
    button->target = target;
    button->mode = mode;
    return button;
}

component_t *label_new(char *text) {
    component_t *label = (component_t *)malloc(sizeof(component_t));
    if (!label)
        return NULL;

    label->text = strdup(text);
    label->target = NULL;
    label->mode = BUTTON_MODE_DEFAULT;
    return label;
}

void component_free(component_t *component) {
    if (!component)
        return;

    free(component->text);
    free(component);
}

void component_draw(component_t *component, int index) {
    if (component->target || component->mode) {
        draw_button(index + 1, component->text);
    } else {
        draw_label(index + 1, component->text);
    }
}

menu_t *menu_new(const char *title) {
    menu_t *menu = (menu_t *)calloc(sizeof(menu_t), 1);
    menu->title = title;
    return menu;
}

void menu_free(menu_t *menu) {
    if (!menu)
        return;

    for (int i = 0; i < TOUCH_COMPONENT_COUNT; i++) {
        if (menu->components[i])
            component_free(menu->components[i]);
    }
    free(menu);
}

void menus_free() {
    menu_free(menu_main);
    menu_free(menu_play);
    menu_free(menu_select_level);
    menu_free(menu_score);
    menu_free(menu_win);
    menu_free(menu_lose);
}

void menu_set_component(menu_t *menu, int index, component_t *component) {
    menu->components[index] = component;
}

void menu_draw(menu_t *menu) {
    draw_background(ILI9341_NAVY);
    tft.setTextSize(3);

    int16_t x, y;
    uint16_t w, h;
    tft.getTextBounds(menu->title, 0, 0, &x, &y, &w, &h);
    tft.setCursor(((tft.width() - w) / 2), 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.println(menu->title);

    for (int i = 0; i < TOUCH_COMPONENT_COUNT; i++) {
        if (menu->components[i]) {
            component_draw(menu->components[i], i);
        }
    }
}

button_mode_t menu_loop(menu_t *menu) {
    // Draw the firt menu upon entering the menu loop.
    menu_draw(menu);

    // Loop on the current menu.
    while (true) {
        int index = menu_await_input();
        component_t *component = menu->components[index];

        // If nothing was clicked, continue waiting.
        if (!component)
            continue;

        // If this starts the game, do that now.
        if (component->mode != BUTTON_MODE_DEFAULT) {
            button_mode_t mode = component->mode;
            menus_free();
            return mode;
        }

        // If it is a menu button, it should go to the next menu.
        if (component->target) {
            menu = component->target;
            menu_draw(menu);
            continue;
        }

        // The only remaining scenario is that it is a label, in which
        // case we do nothing here.
    }
}

int menu_await_input() {
    while (true) {
        // Await a screen touch.
        if (!ts.touched())
            continue;

        // Grab the touch point when the touch is released.
        TS_Point touch_point;
        while (ts.touched() || ts.bufferSize())
            touch_point = ts.getPoint();

        // Rotate the coordinates to match the screen orientation.
        int prev_x = touch_point.x;
        touch_point.x = map(touch_point.y, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
        touch_point.y = map(prev_x, TOUCH_MAX_Y, TOUCH_MIN_Y, 0, tft.height());

        // Check if the touch X falls within the column of (potential) buttons.
        if (touch_point.x > TOUCH_BUTTON_START_X
        && touch_point.x < (TOUCH_BUTTON_START_X + TOUCH_COMPONENT_WIDTH)) {
            // Check if the touch Y also falls within a button.
            for (int i = 1; i < TOUCH_COMPONENT_COUNT + 1; i++) {
                if (touch_point.y > (i * (TOUCH_COMPONENT_HEIGHT + TOUCH_COMPONENT_PADDING)) &&
                touch_point.y < ((i + 1) * TOUCH_COMPONENT_HEIGHT + i * TOUCH_COMPONENT_PADDING)) {
                    // If it does, return the index of the button.
                    return i - 1;
                }
            }
        }
    }
}

void touch_init() {
    if (!ts.begin()) {
        debug("[touch] Couldn't start touchscreen\n");
        while (1);
    }

    // Use the screen in landscape mode.
    tft.setRotation(1);
}

void menus_new() {
    menu_main = menu_new("BOMBERMAN");
    menu_play = menu_new("PLAY GAME");
    menu_select_level = menu_new("SELECT LEVEL");
    menu_score = menu_new("HIGH SCORES");
    menu_lose = menu_new("GAME ENDED");
    menu_win = menu_new("GAME ENDED");

    // menu_main
    menu_set_component(menu_main, 0, button_new("Play", menu_play, BUTTON_MODE_DEFAULT));
    menu_set_component(menu_main, 1, button_new("High scores", menu_score, BUTTON_MODE_DEFAULT));

    // menu_play
    menu_set_component(menu_play, 1, button_new("Multiplayer", NULL, BUTTON_MODE_MULTIPLAYER));
    menu_set_component(menu_play, 0, button_new("Singleplayer", menu_select_level, BUTTON_MODE_DEFAULT));
    menu_set_component(menu_play, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));

    // menu_select_level
    menu_set_component(menu_select_level, 0, button_new("Random", NULL, BUTTON_MODE_SINGLEPLAYER_RANDOM));
    menu_set_component(menu_select_level, 1, button_new("Level 1", NULL, BUTTON_MODE_SINGLEPLAYER_PLUS));
    menu_set_component(menu_select_level, 2, button_new("Level 2", NULL, BUTTON_MODE_SINGLEPLAYER_FULL));
    menu_set_component(menu_select_level, 3, button_new("Back", menu_play, BUTTON_MODE_DEFAULT));

    // menu_score
    // Get the 3 highest scores from EEPROM and display them in a list.
    char label[15];
    for (int i = 0; i < 3; i++) {
        menu_set_component(menu_score, i, label_new(menu_get_score(i, label)));
    }
    menu_set_component(menu_score, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));

    // menu_lose
    menu_set_component(menu_lose, 1, label_new("You lose!"));
    menu_set_component(menu_lose, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));

    // menu_win
    int win_pos = 1;
    if (!game_is_multiplayer()) {
        float score = score_get();
        sprintf(label, "Score: %u", (int)score);
        menu_set_component(menu_win, 2, label_new(label));
        win_pos = 0;
    }

    menu_set_component(menu_win, win_pos, label_new("You win!"));
    menu_set_component(menu_win, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));
}

// Get formatted score from EEPROM.
char *menu_get_score(int index, char *label) {
    uint16_t score = eeprom_get(index);

    sprintf(label, "%u. %u", index+1, score);
    return label;
}
