#include "player.h"
#include "bomb.h"
#include "defines.h"
#include "grid.h"
#include "render.h"
#include "segments.h"

uint8_t on_explosion = 0;

// Create a new player struct.
player_t *player_new(uint8_t x, uint8_t y) {
    player_t *player = (player_t *)malloc(sizeof(player_t));
    if (!player)
        return player;

    player->x = x;
    player->y = y;
    player->lives = 3;
    player->bomb = NULL;
    player->is_hit = 0;
    return player;
}

// Delete a player struct.
void player_free(player_t *player) {
    if (player) {
        if (player->bomb)
            bomb_free(player->bomb);
        free(player);
    }
}

// Process user input and optionally rerender the player.
void player_update(player_t *player, uint8_t inputs) {
    // Make a variable in which the new location of the player will be stored
    uint8_t new_x = player->x;
    uint8_t new_y = player->y;
    uint8_t prev_x;
    uint8_t prev_y;

    // Check if player is walking inside an exploded bomb.
    if (grid_get_cell_type(player->x, player->y) == EXPLODING_BOMB && !player->is_hit) {
        player->is_hit = INVISIBILITY_LENGTH;
        player->lives--;
        on_explosion = 1;

        player_show_lives(player);
        grid_redraw_cell(player->x, player->y, player);
    }

    // If player is currently hit.
    if (player->is_hit) {
        player->is_hit--;

        if (on_explosion && grid_get_cell_type(player->x, player->y) != EXPLODING_BOMB) {
            on_explosion = 0;
            grid_redraw_cell(player->x, player->y, player);
        }

        if (!player->is_hit) {
            grid_redraw_cell(player->x, player->y, player);
        }
    }

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

    // Get the type of the new cell.
    cell_type_t new_cell = grid_get_cell_type(new_x, new_y);

    // Check if the player is allowed to move to the new cell.
    if (new_cell != EMPTY && new_cell != EXPLODING_BOMB)
        return;

    // If we move, redraw the old cell and draw our player over the new cell.
    if (player->x != new_x || player->y != new_y) {
        // Save previous location.
        prev_x = player->x;
        prev_y = player->y;

        // Update the player location.
        player->x = new_x;
        player->y = new_y;

        // Redraw the current cell.
        grid_redraw_cell(prev_x, prev_y, player);

        // Draw the player on the new position.
        draw_player(player);
    }

    // TODO: Place bomb if: inputs & (1 << INPUT_BUTTON_Z).
    if (inputs & (1 << INPUT_BUTTON_Z) && !player->bomb) {
        player_place_bomb(player);
    }

    if (player->bomb) {
        bomb_update(player->bomb);
        if (player->bomb->life_state == DEATH_STATE) {
            //delete bomb
            bomb_explosion_toggle(player->bomb, EMPTY);
            bomb_free(player->bomb);
            player->bomb = NULL;
            return;
        }
    }
}

// Show the lives of the given player on the seven segment display using TWI.
void player_show_lives(player_t *player) {
    segments_show(player->lives);
}

//place a bomb on the map
void player_place_bomb(player_t *player) {
    if (!player->bomb) {                                           //if this player doesn't already have a bomb on the map
        player->bomb = bomb_new(player->x, player->y);             //create a bomb
        grid_change_cell(player->bomb->x, player->bomb->y, BOMB);  //draw the bomb on the map
    }
}
