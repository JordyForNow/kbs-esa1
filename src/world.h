#ifndef WORLD_H
#define WORLD_H

#include "defines.h"

#include <Adafruit_ILI9341.h>

// The possible items a tile can contain.
typedef enum {
    EMPTY,
    BOMB,
    WALL,
    BOX,
    EXPLODING_BOMB,
    UPGRADE_EXPLOSION_BOMB_SIZE,
    UPGRADE_BOMB_SIZE,
    UPGRADE_EXPLOSION_BOMB_COUNT,
    UPGRADE_BOMB_COUNT,
} tile_t;

struct world_t;

#include "player.h"

typedef struct world_t {
    tile_t tiles[WORLD_WIDTH][WORLD_HEIGHT];
    player_t **players;
    uint8_t player_count;
    uint8_t boxes;
} world_t;

world_t *world_new(uint8_t player_count);
void world_free(world_t *world);
void world_generate(world_t *world, unsigned long seed);
uint8_t world_count_boxes(world_t *world);

void world_update(world_t *world, uint8_t inputs);

uint8_t world_set_tile(world_t *world, uint8_t x, uint8_t y, tile_t tile);
void world_subtract_boxes(world_t *world, int subtraction_factor);
int world_get_boxes(world_t *world);
tile_t world_get_tile(world_t *world, uint8_t x, uint8_t y);
void world_redraw_tile(world_t *world, uint8_t x, uint8_t y);
player_t *world_get_player(world_t *world, uint8_t x, uint8_t y);
bomb_t *world_get_bomb(world_t *world, uint8_t x, uint8_t y);
world_t *game_get_world();

#endif /* WORLD_H */
