#ifndef GAME_H
#define GAME_H

#include <nunchuck_funcs.h>
#include <stdbool.h>

typedef enum {
    GAME_STATE_RUNNING,
    GAME_STATE_WON,
    GAME_STATE_LOST,
} game_state_t;

void game_init();
void game_free();
bool game_update();
void game_trigger_update();
game_state_t game_get_state();

#endif
