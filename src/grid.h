#ifndef GRID_H
#define GRID_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "defines.h"

extern Adafruit_ILI9341 tft;

// The possible items a cell can contain.
typedef enum cell_types {
    player_one,
    player_two,
    wall,
    bomb,
    box,
    empty,
    exploding_bomb,
} cellTypes_t;

void draw_cell(int x, int y, cellTypes_t type);
void draw_player_one(int x, int y);
void draw_player_two(int x, int y);
void draw_wall(int x, int y);
void draw_bomb(int x, int y);
void draw_exploding_bomb(int x, int y);
void draw_box(int x, int y);
void draw_empty(int x, int y);

cellTypes_t get_cell_content(int x, int y);

int change_cell(int x, int y, cellTypes_t changeTo);

void grid_init();

#endif /* GRID_H */