#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG 0
#define SEGMENTS_ENABLE 1

// Set to 1 if the TFT-backlight is connected to Arduino PIN 10.
// If the screen is connected directly, set this define to 0.
#define ENABLE_DIMMING_BACKLIGHT 1

// The length to clear the corner from boxes.
#define GAME_STARTING_AREA 4

// The frequency with which the game updates.
#define GAME_UPDATE_FREQUENCY 8

// This many input polls will be done between game updates.
#define GAME_INPUT_FACTOR 25

// The TOP for Timer1. 15625 is for 1Hz. This will be divided by the desired
// update frequency so the timer will generate a signal that is equal to the desired frequency.
#define TIMER1_TOP (15625 / GAME_UPDATE_FREQUENCY / GAME_INPUT_FACTOR)

// The inputs are read from the nunchuck once during every game-update
// and are stored in a single uint8_t when they are passed around to
// other functions, such as player_update(..). The below defines are
// used as bitmasks (for example: 1 << INPUT_JOY_RIGHT).
#define INPUT_JOY_UP 0
#define INPUT_JOY_RIGHT 1
#define INPUT_JOY_DOWN 2
#define INPUT_JOY_LEFT 3
#define INPUT_BUTTON_Z 4
#define INPUT_BUTTON_C 5

// The threshold for activation for the joystick on the nunchuck.
// If the joystick is within INPUT_THRESHOLD from either the lower (0)
// or higher (INPUT_MAX) bound, then the movement will be registered.
#define INPUT_JOY_DEADZONE 10
#define INPUT_JOY_MAX 255

// The percentage of commitment required before moving in a certain direction.
#define INPUT_JOY_THRESHOLD_PERCENTAGE 0.25
#define INPUT_JOY_THRESHOLD (GAME_INPUT_FACTOR * (INPUT_JOY_MAX / 2) * INPUT_JOY_THRESHOLD_PERCENTAGE)

// The size of one tile, possible other size: 14, width: 23, height 17.
#define WORLD_TILE_SIZE 19
#define WORLD_WIDTH 17
#define WORLD_HEIGHT 13

// The properties a tile can have.
#define TILE_MASK_IS_EXPLODING 0b0001
#define TILE_MASK_IS_BOMB 0b0010
#define TILE_MASK_IS_COUNT_UPGRADE 0b1010
#define TILE_MASK_IS_UPGRADE 0b1000

// The maximum amount of bombs a player can have.
#define MAX_BOMB_COUNT 7
// The maximum size the explosion of a bomb can be.
#define MAX_BOMB_SIZE 7
// Chance for a box to drop an upgrade.
#define BOMB_EXPLODE_SIZE_DROP_CHANCE 15
#define BOMB_COUNT_UPGRADE_CHANCE 15
// Defines after how many seconds the bomb should explode.
#define BOMB_EXPLODE_AGE (3 * GAME_UPDATE_FREQUENCY)
// Defines after how many seconds the bomb should destroy.
#define BOMB_DESTROY_AGE (4 * GAME_UPDATE_FREQUENCY)
// Number of how many directions there are.
#define BOMB_DIRECTION_COUNT 4
// How many seconds a player should be invinsible to bombs after being hit.
#define HIT_DURATION (5 * GAME_UPDATE_FREQUENCY)

// The PIN which the screen is connected to.
#define TFT_DC 9
// If the TFT brightness doesn't need to be changd the TFT_CS
// can be connected to PIN 10, otherwise this needs to be PIN 7.
#if ENABLE_DIMMING_BACKLIGHT
#define TFT_CS 7
#else
#define TFT_CS 10
#endif /* ENABLE_DIMMING_BACKLIGHT */

//////////////
//Networking//
//////////////

//Usart settings
#define USART_ENABLED 1
#define USART_BAUD_RATE 115200

// Extra colors to use in the screen.
#define ILI9341_BROWN 0x6100

// The STMPE610 uses hardware SPI on the shield, and #8.
#define STMPE_CS 8

// This is calibration data for the raw touch data to the screen coordinates.
#define TOUCH_MIN_X 140
#define TOUCH_MIN_Y 150
#define TOUCH_MAX_X 3900
#define TOUCH_MAX_Y 4000

// The amount of buttons or labels a menu screen can have.
#define TOUCH_COMPONENT_COUNT 4

// The width, height, and padding of components.
#define TOUCH_COMPONENT_HEIGHT 40
#define TOUCH_COMPONENT_WIDTH 200
#define TOUCH_COMPONENT_PADDING 10

// The horizontal start position of buttons.
#define TOUCH_BUTTON_START_X 60

// Extra color(s) to use in the screen.
#define ILI9341_BROWN 0x6100

// Logging macros that only do something when DEBUG is enabled.
// They print messages in the following style: [src/game.cpp] updating().
// It includes the source file where the logging call originated from.
#if DEBUG
// If DEBUG is enabled, enable logging.
#define LOG_INIT() Serial.begin(115200);
#define LOG(msg) Serial.print("[" __FILE__ "] " msg);
#define LOGLN(msg) Serial.println("[" __FILE__ "] " msg);
#else
// If DEBUG is disabled, don't log anything.
#define LOG_INIT() {};
#define LOG(msg) {};
#define LOGLN(msg) {};
#endif /* DEBUG */

#endif /* DEFINES_H */
