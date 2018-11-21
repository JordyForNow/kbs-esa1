#include "player.h"
#include <stdlib.h>
#include "segments.h"
#include "bomb.h"

// Create a new player struct.
player_t *player_new(uint8_t x, uint8_t y) {
    player_t *player = (player_t*) malloc(sizeof(player_t));
    if (player == NULL)
        return player;

    player->x = x;
    player->y = y;
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

    if(player->bomb){
        bomb_update(player->bomb);
    }
}

// Show the lives of the given player on the seven segment display using TWI.
void player_show_lives(player_t *player) {
    segments_show(player->lives);
}

void player_place_bomb(player_t *player){
    player->bomb = bomb_new(player->x, player->y);
}
