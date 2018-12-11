#include "packet.h"
#include <Arduino.h>
#include "defines.h"
#include "network.h"

packet_t* packet_new(method_t method, uint8_t x, uint8_t y) {
    packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
    packet->method = method;
    packet->x = x;
    packet->y = y;

    return packet;
}

packet_t* packet_new(uint16_t seed) {
    packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
    packet->method = INIT;
    packet->seed = seed;

    return packet;
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

    // Look at the packet as if it's an uint16_t.
    uint16_t *raw = (uint16_t*) &packet;

    // Determine the value of the parity bit.
    packet.parity = !has_even_parity(*raw);

    // Transmit the packet.
    network_send(*raw);
}

// Send communication packet with map seed.
void packet_setup(uint16_t map_seed) {
    // Declare packet variable equal to given map seed.
    packet_t packet;
    packet.method = INIT;
    packet.seed = map_seed;

    // Look at the packet as if it's an uint16_t.
    uint16_t *raw = (uint16_t*) &packet;

    // Determine the value of the parity bit.
    packet.parity = !has_even_parity(*raw);

    // Transmit the packet.
    network_send(*raw);
}

// Check if packet needs a parity bit.
uint8_t has_even_parity(uint16_t packet) {
    // Count how many bits are high.
    uint8_t total = 0;

    // Loop through bits in packet.
    for (int i = 0; i < 16; i++) {
        if (packet & (1 << i))
            total++;
    }

    // If there are an even amount of bits 1 there is no need for a parity bit.
    if (total % 2)
        return 0;

    return 1;
}
