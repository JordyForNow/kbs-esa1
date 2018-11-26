#ifndef RENDER_H
#define RENDER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

#include "defines.h"

extern Adafruit_ILI9341 tft;



void draw_player_one(int x, int y);
void draw_player_two(int x, int y);
void draw_rect(int x, int y, uint16_t color);
void draw_circle(int x, int y, uint16_t color);



#endif /* RENDER_H */
