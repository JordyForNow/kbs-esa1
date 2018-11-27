#ifndef GRID_H
#define GRID_H

#include <Adafruit_ILI9341.h>

// The possible items a cell can contain.
typedef enum {
    EMPTY,
    BOMB,
    WALL,
    BOX,
    EXPLODING_BOMB,
} cell_type_t;


#include "player.h"

void grid_init();
int grid_change_cell(int x, int y, cell_type_t changeTo);
void grid_redraw_cell(int x, int y, player_t *player);
cell_type_t grid_get_cell_type(int x, int y);

#endif /* GRID_H */
