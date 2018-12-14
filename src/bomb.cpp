#include "bomb.h"
#include "defines.h"
#include "player.h"
#include "world.h"

// Addition for x and y axis in every direction.
int bomb_explode_addition[BOMB_DIRECTION_COUNT][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// Create a new bomb struct.
bomb_t *bomb_new(uint8_t x, uint8_t y, uint8_t size) {
    bomb_t *bomb = (bomb_t *)malloc(sizeof(bomb_t));
    if (!bomb)
        return bomb;

    bomb->x = x;
    bomb->y = y;
    bomb->age = 0;
    bomb->bomb_size = size;
    return bomb;
}

// Delete a bomb struct.
void bomb_free(bomb_t *bomb) {
    if (bomb) {
        free(bomb);
    }
}

// Update a bomb.
bomb_t *bomb_update(world_t *world, bomb_t *bomb) {
    if (bomb->age == BOMB_DESTROY_AGE) {
        // Free the bomb the deletion process will be handled within the world_update.
        bomb_free(bomb);
        return NULL;
    } else if (bomb->age == BOMB_EXPLODE_AGE) {
        bomb_explode(world, bomb);
    }
    bomb->age++;
    return bomb;
}

// Change the tile at the given coordinates to the given tile type, taking into account
// that there could be a player on the given tile. This player may receive damage if the
// tile we're changing to is an EXPLODING_BOMB.
void bomb_explode_tile(world_t *world, uint8_t x, uint8_t y, tile_t tile) {
    player_t *player = world_get_player(world, x, y);
    if (player && player_on_hit(player))
        LOGLN("Damage from exploding bomb");

    // Reset the explosion counter of the corresponding tile.
    // X and y - 1 because outer walls are not within array.
    // Add 1 to explosion duration because we are going to remove the explosion at counter 1;
    world_set_explosion_counter(world, x-1, y-1, BOMB_DESTROY_AGE - BOMB_EXPLODE_AGE + 1);

    tile_t current_tile = world_get_tile(world, x, y);
    if (current_tile == BOX) {
        long random_number = random(100);

        // Check if a size power-up should drop.
        if (random_number < BOMB_EXPLODE_SIZE_DROP_CHANCE) {
            tile = UPGRADE_EXPLOSION_BOMB_SIZE;

            // Check if a bomb count power-up should drop.
        } else if (random_number < BOMB_EXPLODE_SIZE_DROP_CHANCE + BOMB_COUNT_UPGRADE_CHANCE) {
            tile = UPGRADE_EXPLOSION_BOMB_COUNT;
        }
        
        // Check if a powerup is currently present and replace it with the exploded version.
    } else if (current_tile == UPGRADE_BOMB_COUNT || current_tile == UPGRADE_EXPLOSION_BOMB_COUNT) {
        tile = UPGRADE_EXPLOSION_BOMB_COUNT;
    } else if (current_tile == UPGRADE_BOMB_SIZE || current_tile == UPGRADE_EXPLOSION_BOMB_SIZE) {
        tile = UPGRADE_EXPLOSION_BOMB_SIZE;
    }

    world_set_tile(world, x, y, tile);
}

// The action variable given with this function will determine whether to show or hide the explosion.
void bomb_explode(world_t *world, bomb_t *bomb) {
    tile_t tile = EXPLODING_BOMB;
    // Change bombs location to exploded.
    bomb_explode_tile(world, bomb->x, bomb->y, tile);

    // Loop through directions.
    for (int i = 0; i < BOMB_DIRECTION_COUNT; i++) {
        // Set default location to the location of the bomb.
        uint8_t x_temp = bomb->x;
        uint8_t y_temp = bomb->y;

        // Loop to max explosion size.
        for (int j = 0; j < bomb->bomb_size; j++) {
            // Convert location to a tile within the explosion radius.
            x_temp += bomb_explode_addition[i][0];
            y_temp += bomb_explode_addition[i][1];
            tile_t tile_temp = world_get_tile(world, x_temp, y_temp);

            if (tile_temp == WALL) {
                // A wall can't be broken.
                break;
            } else if (tile_temp == BOX) {
                // If the tile should become an EXPLODING_BOMB and the tile is a box:
                // - Subtract 1 from the total number of boxes.
                // - After a box the explosion should stop.
                if (tile == EXPLODING_BOMB) {
                    bomb_explode_tile(world, x_temp, y_temp, tile);
                    world_subtract_boxes(world, 1);
                }
                break;
            }
            bomb_explode_tile(world, x_temp, y_temp, tile);
        }
    }
}
