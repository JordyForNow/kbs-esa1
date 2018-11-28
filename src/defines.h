#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG 1
#define SEGMENTS_ENABLE 1

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
#define INPUT_JOY_THRESHOLD 50
#define INPUT_JOY_MAX 255

// The size of one tile, possible other size: 14, width: 23, height 17.
#define WORLD_TILE_SIZE 19
#define WORLD_WIDTH 17
#define WORLD_HEIGHT 13

// The length to clear the corner from boxes.
#define GAME_STARTING_AREA 4

// The frequency with which the game updates.
#define GAME_UPDATE_FREQUENCY 5

// Defines how large a bombs explosion should be.
#define BOMB_EXPLODE_SIZE 2
// Defines after how many seconds the bomb should explode.
#define BOMB_EXPLODE_AGE (3 * GAME_UPDATE_FREQUENCY)
// Defines after how many seconds the bomb should destroy.
#define BOMB_DESTROY_AGE (4 * GAME_UPDATE_FREQUENCY)
// Number of how many directions there are.
#define BOMB_DIRECTION_COUNT 4
// How long a player should be invinsible to bombs after being hit.
#define HIT_DURATION 25

// The ports which the screen is connected to.
#define TFT_CS 10
#define TFT_DC 9

// Logging macros that only do something when DEBUG is enabled.
// They print messages in the following style: [src/game.cpp] updating()
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
#endif

#endif
