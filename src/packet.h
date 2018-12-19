#ifndef PACKET_H
#define PACKET_H
#include "player.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum __attribute__ ((__packed__)) {
    MOVE = 0b000,
    LOSE_LIVE = 0b010,
    PLACE_BOMB = 0b001,
    INIT = 0b100,
} identifier_t;

typedef struct {
    identifier_t id : 3;
    union {
        // Used when initialising a multiplayer game.
        uint16_t seed : 10;

        // Used for sending the size of a bomb
        uint8_t size;

        // Used in all other scenarios
        struct {
            uint8_t x : 5;
            uint8_t y : 5;
        };
    };
    uint8_t parity : 1;
} packet_t;


void packet_decode(packet_t *p, uint16_t i);
uint16_t packet_encode(packet_t *p);
void packet_free(packet_t packet);
void packet_setup(uint16_t map_seed);
void packet_send(identifier_t method, player_t *player);
void packet_send_bomb(uint8_t size);
uint8_t has_even_parity(uint16_t packet);
#endif /* PACKET_H */
