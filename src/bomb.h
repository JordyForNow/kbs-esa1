#ifndef BOMB_H
#define BOMB_H

#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t lifeState;
} bomb_t;

bomb_t *bomb_new(uint8_t x, uint8_t y);
void bomb_free(bomb_t *bomb);
void bomb_update(bomb_t *bomb);

#endif