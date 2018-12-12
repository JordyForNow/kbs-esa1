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
    location_t **bomb_exploded_tiles;
} bomb_t;

#include "world.h"

bomb_t *bomb_new(uint8_t x, uint8_t y, uint8_t size);
void bomb_free(bomb_t *bomb);
bomb_t *bomb_update(world_t *world, bomb_t *bomb);
void bomb_explosion_toggle(world_t *world, bomb_t *bomb, tile_t action);
void bomb_explosion_toggle_tile(world_t *world, uint8_t x, uint8_t y, tile_t tile, bomb_t *bomb);

#endif /* BOMB_H */
