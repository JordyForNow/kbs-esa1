#include "bomb.h"
#include "defines.h"
#include "grid.h"
#include "player.h"

// Addition for x and y axis in every direction
int bomb_explode_addition[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// Create a new bomb struct.
bomb_t *bomb_new(uint8_t x, uint8_t y) {
    bomb_t *bomb = (bomb_t *)malloc(sizeof(bomb_t));
    if (bomb == NULL)
        return bomb;

    bomb->x = x;
    bomb->y = y;
    bomb->life_state = 0;
    bomb->explode_size = 2;
    return bomb;
}

// Delete a bomb struct.
void bomb_free(bomb_t *bomb) {
    if (bomb)
        free(bomb);
}

// Update a bomb.
void bomb_update(bomb_t *bomb) {
    if (bomb) {
        if (bomb->life_state == EXPLODE_STATE) {
            bomb_explosion_toggle(bomb, EXPLODING_BOMB);
        }
        bomb->life_state++;
    }
    return;
}

// The action variable given with this function will determine whether to show or hide the explosion.
void bomb_explosion_toggle(bomb_t *bomb, cell_type_t action) {
    // Change bombs position to exploded.
    grid_change_cell(bomb->x, bomb->y, action);

    // Loop through directions.
    for (int i = 0; i < DIRECTIONS; i++) {
        // Set default location to the location of the bomb.
        uint8_t x_temp = bomb->x;
        uint8_t y_temp = bomb->y;

        // Loop to max explosion size.
        for (int j = 0; j < bomb->explode_size; j++) {
            // Convert location to exploded area.
            x_temp += bomb_explode_addition[i][0];
            y_temp += bomb_explode_addition[i][1];

            // A wall can't be broken.
            if (grid_get_cell_type(x_temp, y_temp) == WALL) {
                break;
            }
            // After a box the explosion should stop.
            else if (grid_get_cell_type(x_temp, y_temp) == BOX) {
                if (action == EXPLODING_BOMB)
                    grid_change_cell(x_temp, y_temp, action);
                break;
            }

            grid_change_cell(x_temp, y_temp, action);
        }
    }
}
