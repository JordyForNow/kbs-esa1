#ifndef RENDER_H
#define RENDER_H

#include "grid.h"
#include "player.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

extern Adafruit_ILI9341 tft;

void draw_player(player_t *player);
void draw_rect(int x, int y, uint16_t color);
void draw_circle(int x, int y, uint16_t color);

void draw_cell(int x, int y, cell_type_t type);

#endif /* RENDER_H */
