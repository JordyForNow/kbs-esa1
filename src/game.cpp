#include "game.h"
#include "defines.h"
#include "world.h"
#include "player.h"
#include "render.h"
#include "usart.h"
#include "utils.h"

volatile bool should_poll = false;
volatile bool data_recieved = false;
static int should_update = 0;
static world_t *world;
static player_t *player;

static uint8_t input_buttons = 0;
static int16_t input_joy_x = 0;
static int16_t input_joy_y = 0;

// Initialize the game state.
void game_init() {
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
    usart_init();
}

int validate_incoming_data(uint16_t data){
    return has_even_parity(data);
}

// Update the game, or do nothing if an update hasn't been triggered.
bool game_update() {
    // Handle networking
    if (data_recieved)
    {
        data_recieved = false;

        uint16_t data = usart_get_recieved_bytes();

        if (validate_incoming_data(data))
        {
            usart_send_acknowledgement();

            // usart_send_debug_message("Data received and acknowledged\n");
        }
        else
        {
            // usart_send_debug_message("Data does not have even parity\n");
        }
    }

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

    // Update the world.
    world_update(world, inputs);

    return true;
}

// Trigger a game-update the next time game_update() is called.
void game_trigger_update() {
    should_poll = true;
}

void game_trigger_network_update() {
    data_recieved = true;
}
