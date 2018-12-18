#include "packet.h"
#include "defines.h"
#include "network.h"

inline void packet_send(packet_t packet) {
    // Look at the packet as if it's an uint16_t.
    uint16_t raw = packet_encode(&packet);

    // Determine the value of the parity bit.
    raw |= has_even_parity(raw) ? 0 : 1;

    // Transmit the packet.
    network_send(raw);
}

uint16_t packet_encode(packet_t *p) {
    return p->method == INIT 
    ? (p->method << 11) | (p->seed << 1) | (p->parity) 
    : (p->method << 11) + (p->x << 6) + (p->y << 1) + (p->parity);
}

void packet_decode(packet_t *p, uint16_t i) {
    p->method = (method_t) ((i >> 11) & 0b111);

    if (p->method == INIT) {
        p->seed = (i >> 1) & 0b1111111111;
    } else {
        p->x = (i >> 6) & 0b11111;
        p->y = (i >> 1) & 0b11111;
    }

    p->parity = i & 0b1;
}

void packet_free(packet_t* packet) {
    if (packet)
        free(packet);
}

// Send a packet with a method and the location of the player.
void packet_send(method_t method, uint8_t x, uint8_t y) {
    // Construct the packet.
    packet_t packet;
    packet.method = method;
    packet.x = x;
    packet.y = y;
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
