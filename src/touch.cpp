#include "touch.h"
#include "defines.h"
#include "render.h"
#include "score.h"

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

menu_t *menu_main = NULL;
menu_t *menu_play = NULL;
menu_t *menu_score = NULL;
menu_t *menu_win = NULL;
menu_t *menu_lose = NULL;

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

    free(menu);
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
        if (component->mode != BUTTON_MODE_DEFAULT)
            return component->mode;

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
        LOGLN("Couldn't start touchscreen");
        while (1);
    }
    LOGLN("Touchscreen started");

    // Use the screen in landscape mode.
    tft.setRotation(1);
}

void menus_init() {
    menu_main = menu_new("BOMBERMAN");
    menu_play = menu_new("PLAY GAME");
    menu_score = menu_new("HIGH SCORES");
    menu_lose = menu_new("GAME ENDED");
    menu_win = menu_new("GAME ENDED");

    // menu_main
    menu_set_component(menu_main, 0, button_new("Play", menu_play, BUTTON_MODE_DEFAULT));
    menu_set_component(menu_main, 1, button_new("High scores", menu_score, BUTTON_MODE_DEFAULT));

    // menu_play
    menu_set_component(menu_play, 0, button_new("Singleplayer", NULL, BUTTON_MODE_SINGLEPLAYER));
    menu_set_component(menu_play, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));

    // menu_score
    int index = 1;
    for (uint8_t i=0; i<6; i+=2) {
        uint8_t first = eeprom_read_byte(i);
        uint8_t second = eeprom_read_byte(i+1);
        
        char label[1];
        sprintf(label, "%u. %u,%u", index, first, second);
        menu_set_component(menu_score, (i+1)/2, label_new(label));
        index++;
    }
    menu_set_component(menu_score, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));

    // menu_lose
    menu_set_component(menu_lose, 1, label_new("You lose!"));
    menu_set_component(menu_lose, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));

    // menu_win
    menu_set_component(menu_win, 1, label_new("You win!"));
    menu_set_component(menu_win, 3, button_new("Back", menu_main, BUTTON_MODE_DEFAULT));
}
