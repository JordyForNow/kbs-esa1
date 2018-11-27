#include "grid.h"
#include "defines.h"
#include "render.h"

// The array where the grid items are stored.
cell_type_t grid_layout[GRID_WIDTH][GRID_HEIGHT];

// Function to initialise the two-dimensional array and draw the walls.
void grid_init() {
    // TODO: Find better source of randomness.
    randomSeed(TCNT0);

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (x == 0 || x == (GRID_WIDTH - 1) || y == 0 || y == (GRID_HEIGHT - 1)) {
                // Make vertical and horizontal walls.
                grid_change_cell(x, y, WALL);
            } else if (y > 0 && y < (GRID_HEIGHT - 1) && x > 0 && x < (GRID_WIDTH - 1)) {
                // If it isn't a sidewall, put walls in the field or put some boxes in the field.
                if (y % 2 == 0 && x % 2 == 0) {
                    grid_change_cell(x, y, WALL);
                } else if (random(0, 2)) {
                    grid_change_cell(x, y, BOX);
                }
            }
        }
    }

    // Clear the corners so a player can start and doesn't directly get hit by a bomb.
    for (int i = 1; i < GAME_STARTING_AREA; i++) {
        // Clear top-left corner.
        grid_change_cell(i, 1, EMPTY);
        grid_change_cell(1, i, EMPTY);

        // Clear bottom-right corner.
        grid_change_cell((GRID_WIDTH - 1 - i), (GRID_HEIGHT - 2), EMPTY);
        grid_change_cell((GRID_WIDTH - 2), (GRID_HEIGHT - 1 - i), EMPTY);
    }
}

// Function used to change the cell. Returns 1 if succeeded, 0 if failed because cell is a wall.
int grid_change_cell(int x, int y, cell_type_t change_to) {
    if (grid_layout[x][y] != WALL) {
        grid_layout[x][y] = change_to;
        draw_cell(x, y, change_to);
        return 1;
    }
    return 0;
}

// Redraws the cell at the given position.
void grid_redraw_cell(int x, int y, player_t *player) {
    draw_cell(x, y, grid_layout[x][y]);
    // If there is a bomb on this cell, draw it.
    if (player->bomb->x == x && player->bomb->y == y && player->bomb->life_state < EXPLODE_STATE) {
        draw_cell(x, y, BOMB);
    }
    // If the player is on this cell, draw it.
    if (player->x == x && player->y == y) {
        draw_player(player);
    }
}

cell_type_t grid_get_cell_type(int x, int y) {
    return grid_layout[x][y];
}
