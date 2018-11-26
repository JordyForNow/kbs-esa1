#include "grid.h"

cell_type_t grid_layout[GRID_WIDTH][GRID_HEIGHT];  // The array where the grid items are stored.

// Function to initialise the two-dimensional array and draw the walls.
void grid_init() {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {  // Make vertical walls.
            if (j == 0 || j == (GRID_WIDTH - 1)) {
                change_cell(j, i, WALL);
            }
            if (i == 0 || i == (GRID_HEIGHT - 1)) {  // Make horizontal walls.
                change_cell(j, i, WALL);
            }

            if (i > 0 && i < (GRID_HEIGHT - 1) && j > 0 && j < (GRID_WIDTH - 1)) {  // If it isn't a sidewall, make a sidewall.
                if (i % 2 == 0 && j % 2 == 0) {
                    change_cell(j, i, WALL);
                } else {  // Make box or make empty
                    grid_layout[j][i] = EMPTY;
                    randomSeed(TCNT0);
                    int box_bool = random(0, 2);
                    if (box_bool) {
                        change_cell(j, i, BOX);
                    } else {
                        grid_layout[j][i] = EMPTY;
                    }
                }
            }
        }
    }
    // Clear the corners so a player can start and doesn't directly get hit by a bomb.
    for (int i = 1; i < 4; i++) {
        int change_width = GRID_WIDTH - 1 - i;
        int change_height = GRID_HEIGHT - 1 - i;

        change_cell(i, 1, EMPTY);  // Clear horizontal y=1
        change_cell(change_width, 1, EMPTY);

        change_cell(i, (GRID_HEIGHT - 2), EMPTY);  // Clear horizontal y=(GRID_HEIGHT-2).
        change_cell(change_width, (GRID_HEIGHT - 2), EMPTY);

        change_cell(1, i, EMPTY);  // Clear verticale x=1.
        change_cell(1, change_height, EMPTY);

        change_cell((GRID_WIDTH - 2), i, EMPTY);  // Clear verticale x=(GRID_WIDTH-2).
        change_cell((GRID_WIDTH - 2), change_height, EMPTY);
    }
}

// Function used to change the cell. returns 1 if succeeded, 0 if failed because cell is a wall.
int change_cell(int x, int y, cell_type_t change_to) {
    if (grid_layout[x][y] != WALL) {
        grid_layout[x][y] = change_to;
        draw_cell(x, y, change_to);
        return 1;
    }
    return 0;
}

cell_type_t get_cell_content(int x, int y) {
    return grid_layout[x][y];
}
