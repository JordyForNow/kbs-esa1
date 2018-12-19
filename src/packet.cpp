#include "packet.h"
#include "defines.h"
#include "network.h"

#define METHOD_POS 11
#define METHOD_SIZE 0b111
#define FIRST_DATA_POS 1
#define SECOND_DATA_POS 6

//This is used for 5 bits data
#define DATA_SIZE 0b11111
//This is used for 10 bits data
#define BIG_DATA_SIZE 0b1111111111

inline void packet_send(packet_t packet) {
    // Look at the packet as if it's an uint16_t.
    uint16_t raw = packet_encode(&packet);

    // Determine the value of the parity bit.
    raw |= has_even_parity(raw) ? 0 : 1;

    // Transmit the packet.
    network_send(raw);
}

uint16_t packet_encode(packet_t *p) {
    switch (p->method) {
        case INIT:
            return (p->method << METHOD_POS) | (p->seed << FIRST_DATA_POS) | (p->parity);
        case PLACE_BOMB: 
            return (p->method << METHOD_POS) | (p->size << FIRST_DATA_POS) | (p->parity);
        default:
            return (p->method << METHOD_POS) + (p->x << SECOND_DATA_POS) + (p->y << FIRST_DATA_POS) + (p->parity);
    }
}

void packet_decode(packet_t *p, uint16_t i) {
    
    // Get the method type from the incoming data
    p->method = (method_t) ((i >> METHOD_POS) & METHOD_SIZE);

    // Fill the packet based on the method type
    switch (p->method) {
        case INIT:
            p->seed = (i >> FIRST_DATA_POS) & BIG_DATA_SIZE;
            break;
        case PLACE_BOMB: 
            p->size = (i >> FIRST_DATA_POS) & DATA_SIZE;
            break;
        default:
            p->x = (i >> SECOND_DATA_POS) & DATA_SIZE;
            p->y = (i >> FIRST_DATA_POS) & DATA_SIZE;
    }

    p->parity = i & 0b1;
}

void packet_free(packet_t* packet) {
    if (packet)
        free(packet);
}

// Send a packet with a method and the location of the player.
void packet_send(method_t method, player_t *player) {
    // Construct the packet.
    packet_t packet;
    packet.method = method;
    packet.x = player->x;
    packet.y = player->y;
    packet.parity = 0;

    packet_send(packet);
}

void packet_send_bomb(uint8_t size) {
     // Construct the packet.
    packet_t packet;
    packet.method = PLACE_BOMB;
    packet.size = size;
    packet.parity = 0;

    packet_send(packet);
}

// Send communication packet with map seed.
void packet_setup(uint16_t map_seed) {
    // Declare packet variable equal to given map seed.
    packet_t packet;
    packet.method = INIT;
    packet.seed = map_seed;
    packet.parity = 0;

    packet_send(packet);
}

// Check if packet needs a parity bit.
uint8_t has_even_parity(uint16_t packet) {
    int count = 1;

    for (int i = 0; i < 16; i++) {
        if (packet & (1 << i))
            count++;
    }

    return count % 2;
}
