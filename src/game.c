#include "game.h"
#include <stdbool.h>

static bool should_update = false;

// Initialize the game state.
void game_init() {
}

// Update the game, or do nothing if an update hasn't been triggered.
void game_update() {
    // Don't update unless our timer tells us it's time.
    if (!should_update)
        return;

    // Make sure we don't keep updating.
    should_update = false;

    // TODO: Handle input.
    // TODO: Handle networking.
    // TODO: Update all entities.
}

// Trigger a game update the next time game_update() is called.
void game_trigger_update() {
    should_update = true;
}
