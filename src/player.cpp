#include "player.h"
#include "bomb.h"
#include "defines.h"
#include "render.h"
#include "segments.h"
#include "world.h"
#include "game.h"

// Create a new player struct.
player_t *player_new(uint8_t x, uint8_t y, uint8_t is_main) {
    player_t *player = (player_t *)malloc(sizeof(player_t));
    if (!player)
        return player;

    player->x = x;
    player->y = y;
    player->lives = 3;
    player->bombs = (bomb_t **)calloc(sizeof(bomb_t *), MAX_BOMB_COUNT);
    player->hit_duration = 0;
    player->is_main = is_main;
    player->bomb_count = 1;
    player->bomb_size = 2;
    return player;
}

// Delete a player struct.
void player_free(player_t *player) {
    if (!player)
        return;

    // Free any bombs that may currently be placed on the screen.
    for (int i = 0; i < MAX_BOMB_COUNT; i++) {
        bomb_free(player->bombs[i]);
    }

    free(player);
}

void player_increment_bomb_size(player_t *player) {
    // Check if the incrementation is allowed.
    if (player->bomb_size < MAX_BOMB_SIZE)
        player->bomb_size++;
}

void player_increment_bomb_count(player_t *player) {
    // Check if the incrementation is allowed.
    if (player->bomb_count < MAX_BOMB_COUNT)
        player->bomb_count++;
}

uint8_t player_move(player_t *player, uint8_t inputs, world_t *world, uint8_t redraw) {
    // Check where we are going.
    uint8_t new_x = player->x;
    uint8_t new_y = player->y;

    // Process user input.
    if (inputs & (1 << INPUT_JOY_LEFT)) {
        new_x--;
    } else if (inputs & (1 << INPUT_JOY_RIGHT)) {
        new_x++;
    } else if (inputs & (1 << INPUT_JOY_UP)) {
        new_y--;
    } else if (inputs & (1 << INPUT_JOY_DOWN)) {
        new_y++;
    }

    // Check the tile where we are going.
    tile_t new_tile = world_get_tile(world, new_x, new_y);

    // Check if we want to and can move into the new tile.
    if ((new_x != player->x || new_y != player->y)
    && (new_tile != WALL && new_tile != BOX && new_tile != BOMB)) {
        // Store where we were, so we can rerender the tile once we've moved.
        uint8_t old_x = player->x;
        uint8_t old_y = player->y;

        // Update the player position.
        player->x = new_x;
        player->y = new_y;

        // If next position is a power-up.
        if (new_tile & TILE_MASK_IS_UPGRADE) {
            // If the next position is a size power-up.
            if (new_tile & TILE_MASK_IS_SIZE_UPGRADE) {
                player_increment_bomb_size(player);
            } else {
                player_increment_bomb_count(player);
            }
        }

        bool exploding = false;
        
        // If next position is exploded.
        if (new_tile & TILE_MASK_IS_EXPLODING) {
            exploding = true;
            // Remove a life if the player is not invincible.
            player_on_hit(player);
        }

        world_set_tile(world, new_x, new_y, exploding ? EXPLODING_BOMB : EMPTY);

        // Rerender the tile we came from, and render the player on top of the new tile.
        world_redraw_tile(world, old_x, old_y);
        redraw = 1;
    } else if (world_get_tile(world, player->x, player->y) == EXPLODING_BOMB && player_on_hit(player)) {
        // If we don't want to move or we are unable to, we should check if we
        // are standing inside an explosion. If we are, we might have to take damage.
        LOGLN("Damage from standing in explosion");
    }

    return redraw;
}

// Process user input and optionally rerender the player.
void player_update(world_t *world, player_t *player, uint8_t inputs) {
    uint8_t redraw = 0;

    // Decrease our invincibility.
    if (player->hit_duration) {
        player->hit_duration--;

        // If we are no longer invincible, redraw.
        if (!player->hit_duration) {
            redraw = 1;
        }
    }

    redraw = player_move(player, inputs, world, redraw);

    // Place a bomb if necessary.
    int bomb_index = bomb_allowed(player, world);
    if (bomb_index < MAX_BOMB_COUNT && inputs & (1 << INPUT_BUTTON_C)) {
        player_place_bomb(world, player, bomb_index);
        redraw = 1;
    }

    // Redraw our player only if we have to.
    if (redraw)
        draw_player(player);
}

// Whenever the player should take damage, we check if they are invincible and
// deal the damage if they are not.
uint8_t player_on_hit(player_t *player) {
    if (player->hit_duration)
        return 0;

    player->hit_duration = HIT_DURATION;
    if (player->lives)
        player->lives--;

    player_show_lives(player);
    return 1;
}

// Show the lives of the given player on the seven segment display using TWI.
void player_show_lives(player_t *player) {
    segments_show(player->lives);
}

uint8_t bomb_allowed(player_t *player, world_t *world) {
    if (world_get_tile(world, player->x, player->y) == BOMB)
        return MAX_BOMB_COUNT;
    for (int i = 0; i < player->bomb_count; i++) {
        if (!player->bombs[i]) {
            return i;
        }
    }
    return MAX_BOMB_COUNT;
}

// Place a bomb on the map if the player doesn't already have one.
void player_place_bomb(world_t *world, player_t *player, uint8_t bomb_index) {
    player->bombs[bomb_index] = bomb_new(player->x, player->y, player->bomb_size);
    world_set_tile(world, player->bombs[bomb_index]->x, player->bombs[bomb_index]->y, BOMB);
}
