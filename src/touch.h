#ifndef TOUCH_H
#define TOUCH_H

#include <Adafruit_STMPE610.h>
#include <SoftwareSerial.h>

struct menu_t;

typedef enum {
    BUTTON_MODE_DEFAULT,
    BUTTON_MODE_SINGLEPLAYER,
    BUTTON_MODE_MULTIPLAYER,
} button_mode_t;

typedef struct {
    char *text;
    menu_t *target;
    button_mode_t mode;
} component_t;

typedef struct menu_t {
    const char *title;
    component_t *components[4];
} menu_t;

extern menu_t *menu_main;
extern menu_t *menu_play;
extern menu_t *menu_highscores;

component_t *button_new(char *text, menu_t *target, uint8_t start);
component_t *label_new(char *text);
void component_free(component_t *component);
void component_draw(component_t *component, int index);

menu_t *menu_new(const char *title);
void menu_free(menu_t *menu);

void menu_draw(menu_t *menu);
button_mode_t menu_loop(menu_t *menu);
int menu_await_input();

void touch_init();
void menus_init();

#endif /* TOUCH_H */
