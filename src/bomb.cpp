#include "bomb.h"
#include "defines.h"
#include "world.h"
#include "player.h"

// Addition for x and y axis in every direction
int bomb_explode_addition[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// Create a new bomb struct.
bomb_t *bomb_new(uint8_t x, uint8_t y) {
    bomb_t *bomb = (bomb_t *)malloc(sizeof(bomb_t));
    if (!bomb)
        return bomb;

    bomb->x = x;
    bomb->y = y;
    bomb->age = 0;
    return bomb;
}

// Delete a bomb struct.
void bomb_free(bomb_t *bomb) {
    if (bomb)
        free(bomb);
}

// Update a bomb.
bomb_t *bomb_update(world_t *world, bomb_t *bomb) {
    if (bomb->age == BOMB_DESTROY_AGE) {
        // Delete the bomb when we should.
        bomb_explosion_toggle(world, bomb, EMPTY);
        bomb_free(bomb);
        return NULL;
    } else if (bomb->age == BOMB_EXPLODE_AGE) {
        bomb_explosion_toggle(world, bomb, EXPLODING_BOMB);
    }
    bomb->age++;
    return bomb;
}

// Change the tile at the given coordinates to the given tile type, taking into account
// that there could be a player on the given tile. This player may receive damage if the
// tile we're changing to is an EXPLODING_BOMB.
void bomb_explosion_toggle_tile(world_t *world, uint8_t x, uint8_t y, tile_t tile) {
    if (tile == EXPLODING_BOMB) {
        player_t *player = world_get_player(world, x, y);
        if (player && player_on_damage(player)) {
            LOGLN("Damage from exploding bomb");
        }
    }
    world_set_tile(world, x, y, tile);
}

// The action variable given with this function will determine whether to show or hide the explosion.
void bomb_explosion_toggle(world_t *world, bomb_t *bomb, tile_t tile) {
    // Change bombs location to exploded.
    bomb_explosion_toggle_tile(world, bomb->x, bomb->y, tile);

    // Loop through directions.
    for (int i = 0; i < BOMB_DIRECTION_COUNT; i++) {
        // Set default location to the location of the bomb.
        uint8_t x_temp = bomb->x;
        uint8_t y_temp = bomb->y;

        // Loop to max explosion size.
        for (int j = 0; j < BOMB_EXPLODE_SIZE; j++) {
            // Convert location to a tile within the explosion radius.
            x_temp += bomb_explode_addition[i][0];
            y_temp += bomb_explode_addition[i][1];
            tile_t tile_temp = world_get_tile(world, x_temp, y_temp);

            if (tile_temp == WALL) {
                // A wall can't be broken.
                break;
            } else if (tile_temp == BOX) {
                // After a box the explosion should stop.
                if (tile == EXPLODING_BOMB)
                    bomb_explosion_toggle_tile(world, x_temp, y_temp, tile);
                break;
            }

            bomb_explosion_toggle_tile(world, x_temp, y_temp, tile);
        }
    }
}
