#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdbool.h>
#include "player.h"

typedef enum {
    MOVE,
    LOSE_LIVE,
    PLACE_BOMB,
} method_t;

void packet_setup(uint16_t map_seed);
void packet_send(method_t method, player_t *player);
uint8_t has_even_parity(uint16_t packet);

#endif /* PACKET_H */