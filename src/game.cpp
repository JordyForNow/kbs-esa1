#include "game.h"
#include "player.h"
#include "nunchuck_funcs.h"
#include "defines.h"
#include <stdbool.h>

volatile bool should_update = false;
static player_t *player;

// Initialize the game state.
void game_init() {
    // Create the player and show the lives on the 7-segment display.
    player = player_new();
    player_show_lives(player); // Never updated, so this is fine.

    // Initialize the nunchuck.
    nunchuck_send_request();
}

// Update the game, or do nothing if an update hasn't been triggered.
bool game_update() {

    // Don't update unless our timer tells us it's time.
    if (!should_update)
        return false;

    // Make sure we don't keep updating.
    LOGLN("updating()");
    should_update = false;

    // Read the inputs and store them as a single byte.
    uint8_t inputs = 0;
    if (nunchuck_get_data()) {
        uint8_t x = nunchuck_joyx();
        uint8_t y = nunchuck_joyy();
        inputs |= (x < 50) << INPUT_LEFT;
        inputs |= (x > 205) << INPUT_RIGHT;
        inputs |= (y < 50) << INPUT_DOWN;
        inputs |= (y > 205) << INPUT_UP;
        inputs |= (nunchuck_zbutton()) << INPUT_BUTTON_Z;
        inputs |= (nunchuck_cbutton()) << INPUT_BUTTON_C;
    }

    // TODO: Handle networking.

    // Update all entities.
    player_update(player, inputs);

    return true;
}

// Trigger a game update the next time game_update() is called.
void game_trigger_update() {
    should_update = true;
}
