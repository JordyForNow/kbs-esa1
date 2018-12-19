#include "packet.h"
#include "defines.h"
#include "network.h"

#define ID_POS 11
#define ID_SIZE 0b111
#define FIRST_DATA_POS 1
#define SECOND_DATA_POS 6

// This is used for 5 bits data.
#define DATA_SIZE 0b11111
// This is used for 10 bits data.
#define BIG_DATA_SIZE 0b1111111111

inline void packet_send(packet_t packet) {
    // Look at the packet as if it's an uint16_t.
    uint16_t raw = packet_encode(&packet);

    // Determine the value of the parity bit.
    raw |= !has_even_parity(raw);

    // Transmit the packet.
    network_send(raw);
}

uint16_t packet_encode(packet_t *p) {
    switch (p->id) {
        case INIT:
            return (p->id << ID_POS) | (p->seed << FIRST_DATA_POS) | (p->parity);
        case PLACE_BOMB: 
            return (p->id << ID_POS) | (p->size << FIRST_DATA_POS) | (p->parity);
        default:
            return (p->id << ID_POS) + (p->x << SECOND_DATA_POS) + (p->y << FIRST_DATA_POS) + (p->parity);
    }
}

void packet_decode(packet_t *p, uint16_t to_decode) {
    
    // Get the method type from the incoming data.
    p->id = (identifier_t) ((to_decode >> ID_POS) & ID_SIZE);

    // Fill the packet based on the method type.
    switch (p->id) {
        case INIT:
            p->seed = (to_decode >> FIRST_DATA_POS) & BIG_DATA_SIZE;
            break;
        case PLACE_BOMB: 
            p->size = (to_decode >> FIRST_DATA_POS) & DATA_SIZE;
            break;
        default:
            p->x = (to_decode >> SECOND_DATA_POS) & DATA_SIZE;
            p->y = (to_decode >> FIRST_DATA_POS) & DATA_SIZE;
    }

    p->parity = to_decode & 0b1;
}

void packet_free(packet_t* packet) {
    if (packet)
        free(packet);
}

// Send a packet with a method and the location of the player.
void packet_send(identifier_t identifier, player_t *player) {
    // Construct the packet.
    packet_t packet;
    packet.id = identifier;
    packet.x = player->x;
    packet.y = player->y;
    packet.parity = 0;

    packet_send(packet);
}

void packet_send_bomb(uint8_t size) {
     // Construct the packet.
    packet_t packet;
    packet.id = PLACE_BOMB;
    packet.size = size;
    packet.parity = 0;

    packet_send(packet);
}

// Send communication packet with map seed.
void packet_setup(uint16_t map_seed) {
    // Declare packet variable equal to given map seed.
    packet_t packet;
    packet.id = INIT;
    packet.seed = map_seed;
    packet.parity = 0;

    packet_send(packet);
}

uint8_t has_even_parity(uint16_t packet) {
    int count = 1;

    for (int to_decode = 0; to_decode < 16; to_decode++) {
        if (packet & (1 << to_decode))
            count++;
    }

    return count % 2;
}
