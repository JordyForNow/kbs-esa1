#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdbool.h>
#include "player.h"
#include "world.h"

typedef enum __attribute__ ((__packed__)) {
    MOVE = 0b000,
    LOSE_LIVE = 0b010,
    PLACE_BOMB = 0b001,
    INIT = 0b100,
} method_t;

typedef struct {
    method_t method : 3;
    union {
        struct {
            uint8_t x : 5;
            uint8_t y : 5;
        };
        uint16_t seed : 10;
    };
    uint8_t parity : 1;
} packet_t;


void packet_decode(packet_t *p, uint16_t i);
uint16_t packet_encode(packet_t *p);
void packet_free(packet_t packet);
void packet_setup(uint16_t map_seed);
void packet_send(method_t method, player_t *player);
uint8_t has_even_parity(uint16_t packet);
packet_t *new_packet(uint8_t x, uint8_t y, method_t method);

#endif /* PACKET_H */