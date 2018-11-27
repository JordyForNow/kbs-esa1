#include "player.h"
#include "defines.h"
#include "grid.h"
#include "render.h"
#include "segments.h"

// Create a new player struct.
player_t *player_new() {
    player_t *player = (player_t *) malloc(sizeof(player_t));
    if (!player)
        return player;

    player->x = 1;
    player->y = 1;
    player->lives = 3;
    return player;
}

// Delete a player struct.
void player_free(player_t *player) {
    if (player)
        free(player);
}

// Process user input and optionally rerender the player.
void player_update(player_t *player, uint8_t inputs) {
    // Make a variable in which the new location of the player will be stored
    uint8_t new_x = player->x;
    uint8_t new_y = player->y;

    // Process user input.
    if (inputs & (1 << INPUT_JOY_LEFT)) {
        new_x--;
    } else if (inputs & (1 << INPUT_JOY_RIGHT)) {
        new_x++;
    } else if (inputs & (1 << INPUT_JOY_UP)) {
        new_y--;
    } else if (inputs & (1 << INPUT_JOY_DOWN)) {
        new_y++;
    }

    // Get the type of the new cell.
    cell_type_t new_cell = grid_get_cell_type(new_x, new_y);
    
    // Check if the player is allowed to move to the new cell.
    if (new_cell != EMPTY && new_cell != EXPLODING_BOMB)
        return;

    // If we move, redraw the old cell and draw our player over the new cell.
    if (player->x != new_x || player->y != new_y) {
        // Redraw the current cell.
        grid_redraw_cell(player->x, player->y);

        // Update the player location.
        player->x = new_x;
        player->y = new_y;

        // Draw the player on the new position.
        draw_player(player);
    }

    // TODO: Place bomb if: inputs & (1 << INPUT_BUTTON_Z).
}

// Show the lives of the given player on the seven segment display using TWI.
void player_show_lives(player_t *player) {
    segments_show(player->lives);
}
