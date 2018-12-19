#ifndef GAME_H
#define GAME_H

#include "touch.h"
#include "player.h"

#include <stdbool.h>
#include <nunchuck_funcs.h>

typedef enum {
    GAME_STATE_RUNNING,
    GAME_STATE_WON,
    GAME_STATE_LOST,
} game_state_t;

void game_init(button_mode_t game_mode);
void game_free();
bool game_update();
void game_trigger_update();
game_state_t game_get_state();
unsigned long *game_get_time();
player_t *game_get_main_player();

#endif /* GAME_H */
