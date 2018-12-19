#ifndef WORLD_H
#define WORLD_H

#include "defines.h"
#include "touch.h"

#include <Adafruit_ILI9341.h>

// The possible items a tile can contain.
// Fourth bit says it's a power up.
// First bit says it's exploding.
typedef enum {
    EMPTY = 0b0000,
    BOMB = 0b0010,
    WALL = 0b0100,
    BOX = 0b0110,
    EXPLODING_BOMB = 0b0001,
    BOMB_EXPLOSION = 0b0011,
    UPGRADE_EXPLOSION_BOMB_SIZE = 0b1101,
    UPGRADE_BOMB_SIZE = 0b1100,
    UPGRADE_EXPLOSION_BOMB_COUNT = 0b1011,
    UPGRADE_BOMB_COUNT = 0b1010,
} tile_t;

struct world_t;

#include "player.h"

typedef struct world_t {
    uint8_t tiles[WORLD_WIDTH / 2 + 1][WORLD_HEIGHT];
    uint8_t tile_explosion_duration[(WORLD_WIDTH - 1) / 2][WORLD_HEIGHT - 2];
    player_t **players;
    uint8_t player_count;
    uint8_t boxes;
} world_t;

world_t *world_new(uint8_t player_count);
void world_free(world_t *world);
void world_generate(world_t *world, unsigned long seed);
void world_generate(world_t *world, unsigned long seed, button_mode_t mode);
uint8_t world_count_boxes(world_t *world);

void world_update(world_t *world, uint8_t inputs);

uint8_t world_set_tile(world_t *world, uint8_t x, uint8_t y, tile_t tile);
void world_subtract_boxes(world_t *world, int subtraction_factor);
int world_get_box_count(world_t *world);
tile_t world_get_tile(world_t *world, uint8_t x, uint8_t y);
void world_redraw_tile(world_t *world, uint8_t x, uint8_t y);
player_t *world_get_player(world_t *world, uint8_t x, uint8_t y);
uint8_t world_get_explosion_counter(world_t *world, uint8_t x, uint8_t y);
void world_set_explosion_counter(world_t *world, uint8_t x, uint8_t y, uint8_t value);

#endif /* WORLD_H */
