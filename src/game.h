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
void game_trigger_network_update();

void opponent_move(uint8_t x, uint8_t y);
void opponent_place_bomb(uint8_t x, uint8_t y);
void opponent_lose_live(uint8_t x, uint8_t y);

game_state_t game_get_state();

#endif
