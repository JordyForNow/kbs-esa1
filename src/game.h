#ifndef GAME_H
#define GAME_H

#include <nunchuck_funcs.h>
#include <stdbool.h>

void game_init();
bool game_update();
void game_trigger_update();
void game_trigger_network_update();

void opponent_move(uint8_t x, uint8_t y);
void opponent_place_bomb(uint8_t x, uint8_t y);
void opponent_lose_live(uint8_t x, uint8_t y);

#endif
