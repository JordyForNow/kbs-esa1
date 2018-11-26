#ifndef RENDER_H
#define RENDER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

#include "defines.h"
#include "grid.h"

extern Adafruit_ILI9341 tft;

void draw_player(int x, int y, int player);
void draw_rect(int x, int y, uint16_t color);
void draw_circle(int x, int y, uint16_t color);

void draw_cell(int x, int y, cell_type_t type);

#endif /* RENDER_H */
