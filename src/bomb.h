#ifndef BOMB_H
#define BOMB_H

#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t age;
} bomb_t;

#include "world.h"

bomb_t *bomb_new(uint8_t x, uint8_t y);
void bomb_free(bomb_t *bomb);
bomb_t *bomb_update(world_t *world, bomb_t *bomb);
void bomb_explosion_toggle(world_t *world, bomb_t *bomb, tile_t action);

#endif /* BOMB_H */
