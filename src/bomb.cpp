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
    bomb->size = size;
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
        // Free the bomb, the deletion process will be handled within the world_update.
        bomb_free(bomb);
        return NULL;
    } else if (bomb->age == BOMB_EXPLODE_AGE) {
        bomb_explode(world, bomb);
    }
    bomb->age++;
    return bomb;
}

// Change the tile at the given coordinates to exploding, taking into account
// that there could be a player on the given tile. This player will then receive damage.
void bomb_explode_tile(world_t *world, uint8_t x, uint8_t y, bool is_origin) {
    tile_t tile = EXPLODING_BOMB;
    player_t *player = world_get_player(world, x, y);
    if (player && player->is_main && player_on_hit(player))
        LOGLN("Damage from exploding bomb");

    // Reset the explosion counter of the corresponding tile.
    // X and Y - 1, because the outer walls are not within the tile explosion counter array.
    // Add 1 to the explosion duration because we are going to remove the explosion at counter = 1.
    // If the explosion would be removed at 0, all tiles would constantly be redrawn because the default is 0.
    world_set_explosion_counter(world, x - 1, y - 1, BOMB_DESTROY_AGE - BOMB_EXPLODE_AGE + 1);

    tile_t current_tile = world_get_tile(world, x, y);
    if (current_tile & TILE_MASK_IS_UPGRADE || (current_tile & TILE_MASK_IS_BOMB && !is_origin)) {
        // Check if a powerup or a bomb is currently present then replace it with the exploded version.
        // But a bomb shouldn't be replaced if its the origin of the explosion.
        tile = (tile_t)(current_tile & (~BOX) | TILE_MASK_IS_EXPLODING);
    }

    world_set_tile(world, x, y, tile);
}

void bomb_explode(world_t *world, bomb_t *bomb) {
    // Change bombs location to exploded.
    bomb_explode_tile(world, bomb->x, bomb->y, true);

    for (int i = 0; i < BOMB_DIRECTION_COUNT; i++) {
        // Set default location to the location of the bomb.
        uint8_t x_temp = bomb->x;
        uint8_t y_temp = bomb->y;

        for (int j = 0; j < bomb->size; j++) {
            // Convert location to a tile within the explosion radius.
            x_temp += bomb_explode_addition[i][0];
            y_temp += bomb_explode_addition[i][1];

            // Check if location is outside screen.
            if (x_temp > WORLD_WIDTH || x_temp < 0 || y_temp > WORLD_HEIGHT || y_temp < 0)
                continue;

            tile_t tile_temp = world_get_tile(world, x_temp, y_temp);

            if (tile_temp == WALL) {
                // A wall can't be broken.
                break;
            } else if (tile_temp == BOX || tile_temp == UPGRAGE_BOX_BOMB_COUNT || tile_temp == UPGRAGE_BOX_BOMB_SIZE) {
                // Subtract 1 from the total number of boxes and stop the explosion.
                bomb_explode_tile(world, x_temp, y_temp, false);
                world_subtract_boxes(world, 1);
                break;
            }
            bomb_explode_tile(world, x_temp, y_temp, false);
        }
    }
}
