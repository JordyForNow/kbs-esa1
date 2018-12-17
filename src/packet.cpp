#include "packet.h"
#include <Arduino.h>
#include "defines.h"
#include "network.h"

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
void packet_send(method_t method, player_t* player) {
    // Construct the packet.
    packet_t packet;
    packet.method = method;
    packet.x = player->x;
    packet.y = player->y;
    packet.parity = 0;

    // Look at the packet as if it's an uint16_t.
    uint16_t raw = packet_encode(&packet);

    // Determine the value of the parity bit.
    raw |= has_even_parity(raw) ? 0 : 1;

    // Transmit the packet.
    network_send(raw);
}

// Send communication packet with map seed.
void packet_setup(uint16_t map_seed) {
    // Declare packet variable equal to given map seed.
    packet_t packet;
    packet.method = INIT;
    packet.seed = map_seed;
    packet.parity = 0;

    // Look at the packet as if it's an uint16_t.
    uint16_t raw = packet_encode(&packet);

    // Determine the value of the parity bit.
    raw |= has_even_parity(raw) ? 0 : 1;

    // Transmit the packet.
    network_send(raw);
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
