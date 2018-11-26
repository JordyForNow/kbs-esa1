#ifndef GRID_H
#define GRID_H

#include "defines.h"

#include <Adafruit_ILI9341.h>

// The possible items a cell can contain.
typedef enum {
    EMPTY,
    BOMB,
    WALL,
    BOX,
    EXPLODING_BOMB,
} cell_type_t;

void grid_init();
cell_type_t grid_get_cell_type(int x, int y);
int grid_change_cell(int x, int y, cell_type_t changeTo);

#include "render.h"
#endif /* GRID_H */
