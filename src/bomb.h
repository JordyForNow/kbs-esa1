#ifndef BOMB_H
#define BOMB_H

#include <stdint.h>

// Define bomb_t with according variables.
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t life_state;
    uint8_t explode_size;
} bomb_t;

#include "world.h"

bomb_t *bomb_new(uint8_t x, uint8_t y);
void bomb_free(bomb_t *bomb);
void bomb_update(world_t *world, bomb_t *bomb);
void bomb_explosion_toggle(world_t *world, bomb_t *bomb, tile_t action);

#endif /* BOMB_H */
