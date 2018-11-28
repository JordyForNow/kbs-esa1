#ifndef TOUCH_H
#define TOUCH_H

#include <Adafruit_STMPE610.h>
#include <SoftwareSerial.h>

typedef struct touch_menu_page touch_menu_page_t;

typedef struct {
    char *text;
    int position;
    touch_menu_page_t *target_page;
} touch_button_t;

typedef struct touch_menu_page {
    char *title;
    touch_button_t *buttons[4];
} touch_menu_page_t;

void touch_init();

void touch_main();
void touch_handler(touch_menu_page_t *current_page);
void draw_main_menu();
void draw_play_menu();
void draw_highscore_menu();

void create_touch_pages();
void touch_draw_page(touch_menu_page_t *page);

#endif /* TOUCH_H */
