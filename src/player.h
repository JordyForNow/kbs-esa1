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
    bomb_t *bombs[MAX_BOMB_COUNT];
    uint8_t hit_duration;
    uint8_t is_main;
    uint8_t bomb_count;
    uint8_t bomb_size;
} player_t;

player_t *player_new(uint8_t x, uint8_t y, uint8_t is_main);
void player_free(player_t *player);

void player_update(world_t *world, player_t *player, uint8_t inputs);
uint8_t player_on_hit(player_t *player);
void player_show_lives(player_t *player);
int bomb_allowed(player_t *player, world_t *world);
void player_place_bomb(world_t *world, player_t *player, uint8_t bomb_index);

#endif
