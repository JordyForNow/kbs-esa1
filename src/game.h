#ifndef GAME_H
#define GAME_H

#include "grid.h"

#include <stdbool.h>

void game_init();
bool game_update();
void game_trigger_update();

#endif
