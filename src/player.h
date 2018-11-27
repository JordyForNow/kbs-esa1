#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>

struct player_t;

#include "bomb.h"

typedef struct player_t {
    uint8_t x;
    uint8_t y;
    uint8_t lives;
    bomb_t *bomb;
    uint8_t hit_duration;
    uint8_t on_explosion;
} player_t;

player_t *player_new(uint8_t x, uint8_t y);
void player_free(player_t *player);

void player_update(player_t *player, uint8_t inputs);
void player_show_lives(player_t *player);
void player_place_bomb(player_t *player);

#endif
