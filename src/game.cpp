#include "game.h"
#include "player.h"

static bool should_update = false;
static player_t *player;

// Initialize the game state.
void game_init() {
    // Create the player and show the lives on the 7-segment display.
    player = player_new();
    player_show_lives(player); // Never updated, so this is fine.
}

// Update the game, or do nothing if an update hasn't been triggered.
bool game_update() {
    // Don't update unless our timer tells us it's time.
    if (!should_update)
        return false;

    // Make sure we don't keep updating.
    should_update = false;

    // TODO: Handle input.
    // TODO: Handle networking.

    // Update all entities.
    player_update(player);

    return true;
}

// Trigger a game update the next time game_update() is called.
void game_trigger_update() {
    should_update = true;
}
