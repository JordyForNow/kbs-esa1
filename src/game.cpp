#include "game.h"
#include "defines.h"
#include "player.h"
#include "render.h"
#include "world.h"

volatile bool should_poll = false;
static int should_update = 0;
static world_t *world;
static player_t *player;

static uint8_t input_buttons = 0;
static int16_t input_joy_x = 0;
static int16_t input_joy_y = 0;

game_state_t game_state = GAME_STATE_RUNNING;

// Initialize the game state.
void game_init() {
    // Reset variables when a game is restarting.
    game_state = GAME_STATE_RUNNING;

    // Initialize the nunchuck.
    nunchuck_send_request();

    // Draw the world with blocks and walls.
    world = world_new(1);
    world_generate(world, TCNT0);

    // Create the player and show the lives on the 7-segment display.
    player = player_new(1, 1);
    player_show_lives(player);
    draw_player(player);

    world->players[0] = player;
}

void game_free() {
    world_free(world);
}

// Update the game, or do nothing if an update hasn't been triggered.
bool game_update() {
    // Check if the player has died.
    if (player->lives)
        game_state = GAME_STATE_LOST;

    // End the game if there are no boxes remaining.
    if (!world_get_boxes(world))
        game_state = GAME_STATE_WON;

    // Check if the game has finished.
    if (game_state)
        return false;

    // Don't poll or update unless the timer tells us to.
    if (!should_poll)
        return false;

    should_poll = false;
    should_update++;

    // Collect inputs.
    if (nunchuck_get_data()) {
        uint8_t x = nunchuck_joyx();
        uint8_t y = nunchuck_joyy();

        int16_t delta_x = x - (INPUT_JOY_MAX / 2);
        int16_t delta_y = (INPUT_JOY_MAX / 2) - y;

        // Make sure minute movements are not registered (deadzone).
        // We track how much we deviate from the (theoretical) center.
        if (delta_x >= INPUT_JOY_DEADZONE || delta_x <= -INPUT_JOY_DEADZONE)
            input_joy_x += delta_x;
        if (delta_y >= INPUT_JOY_DEADZONE || delta_y <= -INPUT_JOY_DEADZONE)
            input_joy_y += delta_y;

        // Make sure we register button presses.
        input_buttons |= nunchuck_cbutton() << INPUT_BUTTON_C;
        input_buttons |= nunchuck_zbutton() << INPUT_BUTTON_Z;
    }

    // Don't update unless it's time.
    if (should_update < GAME_INPUT_FACTOR)
        return false;

    LOGLN("Updating");
    should_update = 0;

    // Collect the definitive inputs. These are the button inputs
    // and the most significant joystick input in one byte.
    uint8_t inputs = input_buttons;

    // Sign bit mask used to get the absolute value of the X and Y movement.
    uint16_t x_mask = input_joy_x >> 15;
    uint16_t y_mask = input_joy_y >> 15;

    // Determine what axis is more prevalent.
    if (((input_joy_x ^ x_mask) + x_mask) >= ((input_joy_y ^ y_mask) + y_mask)) {
        // The X-axis is more or equally prevalent.
        inputs |= (input_joy_x < -INPUT_JOY_THRESHOLD) << INPUT_JOY_LEFT;
        inputs |= (input_joy_x >  INPUT_JOY_THRESHOLD) << INPUT_JOY_RIGHT;
    } else {
        // The Y-axis is more prevalent.
        inputs |= (input_joy_y < -INPUT_JOY_THRESHOLD) << INPUT_JOY_UP;
        inputs |= (input_joy_y >  INPUT_JOY_THRESHOLD) << INPUT_JOY_DOWN;
    }

    // Reset the input trackers.
    input_buttons = 0;
    input_joy_x = 0;
    input_joy_y = 0;

    // TODO: Handle networking.

    // Update the world.
    world_update(world, inputs);

    return true;
}

game_state_t game_get_state() {
    return game_state;
}

// Trigger a game-update the next time game_update() is called.
void game_trigger_update() {
    should_poll = true;
}
