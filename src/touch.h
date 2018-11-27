#ifndef TOUCH_H
#define TOUCH_H

#include <Adafruit_STMPE610.h>
#include <SoftwareSerial.h>

void touch_init();

void touch_main();
void draw_main_menu();
void draw_play_menu();
void draw_highscore_menu();


#endif /* TOUCH_H */
