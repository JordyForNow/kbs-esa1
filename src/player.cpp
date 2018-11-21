#include "player.h"
#include <stdlib.h>
#include "segments.h"

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
void player_update(player_t *player) {
}

// Show the lives of the given player on the seven segment display using TWI.
void player_show_lives(player_t *player) {
    segments_show(player->lives);
}
