#ifndef BOMB_H
#define BOMB_H

#include "defines.h"

#include <stdint.h>

typedef struct location_t {
    uint8_t x;
    uint8_t y;
} location_t;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t age;
    uint8_t bomb_size;
} bomb_t;

#include "world.h"

bomb_t *bomb_new(uint8_t x, uint8_t y, uint8_t size);
void bomb_free(bomb_t *bomb);
bomb_t *bomb_update(world_t *world, bomb_t *bomb);
void bomb_explode(world_t *world, bomb_t *bomb);
void bomb_explode_tile(world_t *world, uint8_t x, uint8_t y, tile_t tile);

#endif /* BOMB_H */
