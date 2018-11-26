#include "game.h"
#include "player.h"
#include "nunchuck_funcs.h"
#include "defines.h"
#include <stdbool.h>

volatile bool should_update = false;
static player_t *player;

// Initialize the game state.
void game_init() {
    // Initialize the nunchuck.
    nunchuck_send_request();

    // Draw the grid with blocks and walls.
    grid_init();

    // Create the player and show the lives on the 7-segment display.
    player = player_new();
    player_show_lives(player); // Never updated, so this is fine.
    draw_player(player);
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

        // If the joystick is within INPUT_THRESHOLD distance of either
        // the lower or higher boundary on either the X or Y axis, then
        // it will flip the bit for said movement direction HIGH. The
        // '<' and '>' operators return a 1 for true, so we OR that
        // single high bit into the input mask.
        inputs |= (x <  INPUT_THRESHOLD)                << INPUT_LEFT;
        inputs |= (x > (INPUT_MAX - INPUT_THRESHOLD))   << INPUT_RIGHT;
        inputs |= (y <  INPUT_THRESHOLD)                << INPUT_DOWN;
        inputs |= (y > (INPUT_MAX - INPUT_THRESHOLD))   << INPUT_UP;

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
