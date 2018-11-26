#include "player.h"
#include "defines.h"
#include <stdlib.h>
#include "segments.h"
#include <Arduino.h>

// Create a new player struct.
player_t *player_new() {
    player_t *player = (player_t*) malloc(sizeof(player_t));
    if (player == NULL)
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

// Update a player.
void player_update(player_t *player, uint8_t inputs) {

    // Process user input.
    if (inputs & (1 << INPUT_LEFT)) {
        player->x--;
    } else if (inputs & (1 << INPUT_RIGHT)) {
        player->x++;
    } else if (inputs & (1 << INPUT_UP)) {
        player->y--;
    } else if (inputs & (1 << INPUT_DOWN)) {
        player->y++;
    }

    // TODO: Draw player.
    // TODO: Place bomb if: inputs & (1 << INPUT_BUTTON_Z).
}

// Show the lives of the given player on the seven segment display using TWI.
void player_show_lives(player_t *player) {
    segments_show(player->lives);
}
