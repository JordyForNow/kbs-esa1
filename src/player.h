#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t lives;
} player_t;

player_t *player_new();
void player_free(player_t *player);

void player_update(player_t *player);
void player_show_lives(player_t *player);

#endif
