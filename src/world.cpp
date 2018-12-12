#include "world.h"
#include "defines.h"
#include "render.h"

world_t *world_new(uint8_t player_count) {
    world_t *world = (world_t *)calloc(sizeof(world_t), 1);
    if (!world)
        return NULL;

    world->player_count = player_count;
    world->players = (player_t **)calloc(sizeof(player_t *), player_count);
    world->boxes = 0;
    if (!world->players) {
        free(world);
        return NULL;
    }

    return world;
}

void world_free(world_t *world) {
    if (!world)
        return;

    for (int i = 0; i < world->player_count; i++) {
        player_free(world->players[i]);
    }
    free(world);
}

void world_generate(world_t *world, unsigned long seed) {
    randomSeed(seed);

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            if (x == 0 || x == (WORLD_WIDTH - 1) || y == 0 || y == (WORLD_HEIGHT - 1)) {
                // Make vertical and horizontal walls.
                world_set_tile(world, x, y, WALL);
            } else if (y > 0 && y < (WORLD_HEIGHT - 1) && x > 0 && x < (WORLD_WIDTH - 1)) {
                // If it isn't a sidewall, put walls in the field or put some boxes in the field.
                if (y % 2 == 0 && x % 2 == 0) {
                    world_set_tile(world, x, y, WALL);
                } else if (random(0, 2)) {
                    world_set_tile(world, x, y, BOX);
                }
            }
        }
    }

    // Clear the corners so a player can start and doesn't directly get hit by a bomb.
    for (int i = 1; i < GAME_STARTING_AREA; i++) {
        // Clear top-left corner.
        world_set_tile(world, i, 1, EMPTY);
        world_set_tile(world, 1, i, EMPTY);

        // Clear bottom-right corner.
        world_set_tile(world, (WORLD_WIDTH - 1 - i), (WORLD_HEIGHT - 2), EMPTY);
        world_set_tile(world, (WORLD_WIDTH - 2), (WORLD_HEIGHT - 1 - i), EMPTY);
    }

    world->boxes = world_count_boxes(world);
}

uint8_t world_count_boxes(world_t *world) {
    uint8_t total = 0;
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            if (world->tiles[x][y] == BOX) {
                total++;
            }
        }
    }
    return total;
}

void world_update(world_t *world, uint8_t inputs) {
    // Update all bombs first.
    player_t *player;
    for (int i = 0; i < world->player_count; i++) {
        player = world->players[i];
        for(int j=0; j<player->bomb_count; j++){
            if(player->bombs[j] != 0 && player->bombs[j] != NULL){
                if(!bomb_update(world,player->bombs[j]))
                    player->bombs[j] = NULL;
            }
        }
    }

    // Update all players once all bombs have been updated.
    for (int i = 0; i < world->player_count; i++) {
        player_update(world, world->players[i], inputs);
    }
}

uint8_t world_set_tile(world_t *world, uint8_t x, uint8_t y, tile_t tile) {
    // Do not accidentally override walls.
    if (world->tiles[x][y] == WALL)
        return 0;
    
    world->tiles[x][y] = tile;
    world_redraw_tile(world, x, y);
    return 1;
}

void world_subtract_boxes(world_t *world, int subtraction_amount) {
    world->boxes -= subtraction_amount;
}

int world_get_box_count(world_t *world) {
    return world->boxes;
}

tile_t world_get_tile(world_t *world, uint8_t x, uint8_t y) {
    return world->tiles[x][y];
}

void world_redraw_tile(world_t *world, uint8_t x, uint8_t y) {
    draw_tile(x, y, world->tiles[x][y]);

    player_t *player = world_get_player(world, x, y);
    if (player)
        draw_player(player);
}

player_t *world_get_player(world_t *world, uint8_t x, uint8_t y) {
    for (int i = 0; i < world->player_count; i++) {
        player_t *player = world->players[i];
        if (player->x == x && player->y == y)
            return player;
    }
    return NULL;
}

bomb_t *world_get_bomb(world_t *world, uint8_t x, uint8_t y) {
    bomb_t *bomb;
    player_t *player;
    for (int i = 0; i < world->player_count; i++) {
        player = world->players[i];
        for (int j=0; j<player->bomb_count; j++) {
            if (player->bombs[j] != 0 && player->bombs[j] != NULL) {
                bomb = player->bombs[j];
                if (bomb->x == x && bomb->y == y)
                    return bomb;
            }
        }
    }
    return NULL;
}

// Check if another bomb has exploded on a specific tile.
bool world_check_bomb(uint8_t x, uint8_t y, world_t *world) {
    // Keep track of times found because it will also find the bomb wich is checking.
    int total_times_found = 0;
    for (int i = 0; i < world->player_count; i++) {
        for (int j = 0; j < world->players[i]->bomb_count; j++) {
            if (!world->players[i]->bombs[j])
                continue;
            for (int k = 0; k < world->players[i]->bomb_size * BOMB_DIRECTION_COUNT; k++) {
                if (world->players[i]->bombs[j]->bomb_exploded_tiles[k]->x == x &&
                world->players[i]->bombs[j]->bomb_exploded_tiles[k]->y == y)
                    total_times_found++;
                if (total_times_found > 1)
                    return 1;
            }
        }
    }
    return 0;
}
