#ifndef GRID_H
#define GRID_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
//#include <stdint.h>
//#include <stdio.h>

#include "defines.h"

extern Adafruit_ILI9341 tft;

// The possible items a cell can contain.
typedef enum  {
    BOMB,
    WALL,
    BOX,
    EMPTY,
    EXPLODING_BOMB,
} cell_type_t;

void draw_cell(int x, int y, cell_type_t type);
void draw_player_one(int x, int y);
void draw_player_two(int x, int y);
void draw_rect(int x, int y, uint16_t color);
void draw_circle(int x, int y, uint16_t color);

cell_type_t get_cell_content(int x, int y);

int change_cell(int x, int y, cell_type_t changeTo);

void grid_init();

#endif /* GRID_H */