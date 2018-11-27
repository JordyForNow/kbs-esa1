#include "player.h"
#include "defines.h"
#include "grid.h"
#include "render.h"
#include "segments.h"

// Create a new player struct.
player_t *player_new() {
    player_t *player = (player_t*) malloc(sizeof(player_t));
    if (!player)
        return player;

    player->x = 0;
    player->y = 0;
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

    // Store the previous position of the player.
    uint8_t prev_x = player->x;
    uint8_t prev_y = player->y;

    // Process user input.
    if (inputs & (1 << INPUT_JOY_LEFT)) {
        player->x--;
    } else if (inputs & (1 << INPUT_JOY_RIGHT)) {
        player->x++;
    } else if (inputs & (1 << INPUT_JOY_UP)) {
        player->y--;
    } else if (inputs & (1 << INPUT_JOY_DOWN)) {
        player->y++;
    }

    // If we move, redraw the old cell and draw our player over the new cell.
    if (player->x != prev_x || player->y != prev_y) {
        grid_redraw_cell(prev_x, prev_y);
        draw_player(player);
    }

    // TODO: Place bomb if: inputs & (1 << INPUT_BUTTON_Z).
}

// Show the lives of the given player on the seven segment display using TWI.
void player_show_lives(player_t *player) {
    segments_show(player->lives);
}
