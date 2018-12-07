#include "packet.h"
#include <Arduino.h>
#include "defines.h"
#include "network.h"

packet_t* packet_new(method_t method, uint8_t x, uint8_t y) {
    packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
    packet->method = method;
    packet->x = x;
    packet->y = y;
}

void packet_free(packet_t* packet) {
    if (packet)
        free(packet);
}

// Send a packet with a method and the location of the player.
void packet_send(method_t method, player_t* player) {
    // Declare packet variable.
    uint16_t packet = 0;

    // Store player location in sepparete variables.
    uint16_t x = player->x;
    uint16_t y = player->y;

    // Check wich method is used and shift the opcode to the right position.
    switch (method) {
        case MOVE:
            break;
        case LOSE_LIVE:
            packet = 0b010;
            packet <<= 11;
            break;
        case PLACE_BOMB:
            packet = 0b001;
            packet <<= 11;
            break;
    }

    // Shift location to supply easy integration in packet.
    x <<= 6;
    y <<= 1;
    packet |= x;
    packet |= y;

    // Add parity bit if neccesary.
    if (!has_even_parity(packet)) {
        packet |= 1;
    }

    // Send packet to network.
    network_send(packet);
}

// Send communication packet with map seed.
void packet_setup(uint16_t map_seed) {
    // Declare packet variable equal to given map seed.
    uint16_t packet = map_seed;

    // Shift map seed into posistion.
    packet <<= 1;

    // Add opcode for easy recoqnition.
    packet |= (0b100 << 11);

    // Add parity bit if necessary.
    if (!has_even_parity(packet)) {
        packet |= 1;
    }

    // Send packet to network.
    network_send(packet);
}

packet_t* packet_decode(uint16_t to_decode) {
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t opcode = 0;

    // Shift parity bit out
    to_decode >>= 1;
    for (int i = 0; i < 5; i++) {
        // Copy y coördinate.
        if (to_decode & (1 << i))
            y |= (1 << i);

        // Copy x coördinate.
        if (to_decode & (1 << (i + 5)))
            x |= (1 << i);

        // Copy opcode.
        if (to_decode & (1 << (i + 10)))
            opcode |= (1 << i);
    }

    return packet_new((method_t)opcode, x, y);
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
