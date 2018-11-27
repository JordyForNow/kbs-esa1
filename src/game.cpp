#include "game.h"
#include "defines.h"
#include "world.h"
#include "player.h"
#include "render.h"

volatile bool should_update = false;
static world_t *world;
static player_t *player;

// Initialize the game state.
void game_init() {
    // Initialize the nunchuck.
    nunchuck_send_request();

    // Draw the world with blocks and walls.
    world = world_new(1);
    world_generate(world, TCNT0);

    // Create the player and show the lives on the 7-segment display.
    player = player_new(1, 1);
    player_show_lives(player); // Never updated, so this is fine.
    draw_player(player);

    world->players[0] = player;
}

// Update the game, or do nothing if an update hasn't been triggered.
bool game_update() {
    // Don't update unless our timer tells us it's time.
    if (!should_update)
        return false;

    // Make sure we don't keep updating.
    LOGLN("Updating");
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
        inputs |= (x < INPUT_JOY_THRESHOLD)                   << INPUT_JOY_LEFT;
        inputs |= (x > (INPUT_JOY_MAX - INPUT_JOY_THRESHOLD)) << INPUT_JOY_RIGHT;
        inputs |= (y < INPUT_JOY_THRESHOLD)                   << INPUT_JOY_DOWN;
        inputs |= (y > (INPUT_JOY_MAX - INPUT_JOY_THRESHOLD)) << INPUT_JOY_UP;

        inputs |= (nunchuck_zbutton()) << INPUT_BUTTON_Z;
        inputs |= (nunchuck_cbutton()) << INPUT_BUTTON_C;
    }

    // TODO: Handle networking.

    // Update all entities.
    player_update(world, player, inputs);

    return true;
}

// Trigger a game-update the next time game_update() is called.
void game_trigger_update() {
    should_update = true;
}
