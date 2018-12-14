#ifndef TOUCH_H
#define TOUCH_H

#include "defines.h"

#include <Adafruit_STMPE610.h>

// Predeclare menu_t since it has a circular dependency with component_t.
struct menu_t;

// The mode a button uses; DEFAULT is that it navigates to another menu.
// If it is SINGLEPLAYER or MULTIPLAYER then a game will start in the
// given mode.
typedef enum {
    BUTTON_MODE_DEFAULT,
    BUTTON_MODE_MULTIPLAYER,
    BUTTON_MODE_SINGLEPLAYER_RANDOM,
    BUTTON_MODE_SINGLEPLAYER_PLUS,
    BUTTON_MODE_SINGLEPLAYER_FULL,    
} button_mode_t;

// A component can be either a label or button, and is part of a menu.
// If the mode is DEFAULT and the target is specified, it will navigate
// to another menu as mentioned above. If the mode is SINGLEPLAYER or
// MULTIPLAYER then the menu_loop() will return the mode so the game can
// start in the given mode. If the mode is DEFAULT but there is no target,
// then this component is interpreted as a label.
typedef struct {
    char *text;
    menu_t *target;
    button_mode_t mode;
} component_t;

// The menu struct contains a title and a list of components. It represents
// one menu screen.
typedef struct menu_t {
    const char *title;
    component_t *components[TOUCH_COMPONENT_COUNT];
} menu_t;

// The menu screens that exist in the game.
extern menu_t *menu_main;
extern menu_t *menu_play;
extern menu_t *menu_select_level;
extern menu_t *menu_score;
extern menu_t *menu_win;
extern menu_t *menu_lose;

// Component functions.
component_t *button_new(char *text, menu_t *target, button_mode_t mode);
component_t *label_new(char *text);
void component_free(component_t *component);
void component_draw(component_t *component, int index);

// Menu functions.
menu_t *menu_new(const char *title);
void menu_free(menu_t *menu);
void menus_free();

void menu_set_component(menu_t *menu, int index, component_t *component);
void menu_draw(menu_t *menu);
button_mode_t menu_loop(menu_t *menu);
int menu_await_input();

// Initializer functions.
void touch_init();
void menus_new();

char *menu_get_score(int index, char *label);
void component_change_text(component_t *component, int index, char *text);

#endif /* TOUCH_H */
