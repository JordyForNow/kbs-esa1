#include "defines.h"
#include "network.h"
#include "packet.h"

// The position in the uint16 of all the packet elements. 
#define ID_POSITION 11
#define X_POSITION 1
#define Y_POSITION 6
#define SEED_POSITION 1
#define SIZE_POSITION 1

// The masks for all packet elements.
#define ID_MASK 0b111
#define LOCATION_MASK 0b11111
#define BOMB_SIZE_MASK 0b11111
#define SEED_MASK 0b1111111111

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
        case PACKET_INIT:
            return (p->id << ID_POSITION) | (p->seed << SEED_POSITION) | (p->parity);
        case PACKET_PLACE_BOMB: 
            return (p->id << ID_POSITION) | (p->size << SIZE_POSITION) | (p->parity);
        default:
            return (p->id << ID_POSITION) + (p->x << X_POSITION) + (p->y << Y_POSITION) + (p->parity);
    }
}

void packet_decode(packet_t *p, uint16_t to_decode) {
    
    // Get the method type from the incoming data.
    p->id = (identifier_t) ((to_decode >> ID_POSITION) & ID_MASK);

    // Fill the packet based on the method type.
    switch (p->id) {
        case PACKET_INIT:
            p->seed = (to_decode >> SEED_POSITION) & SEED_MASK;
            break;
        case PACKET_PLACE_BOMB: 
            p->size = (to_decode >> SIZE_POSITION) & BOMB_SIZE_MASK;
            break;
        default:
            p->x = (to_decode >> X_POSITION) & LOCATION_MASK;
            p->y = (to_decode >> Y_POSITION) & LOCATION_MASK;
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
    packet.id = PACKET_PLACE_BOMB;
    packet.size = size;
    packet.parity = 0;

    packet_send(packet);
}

// Send communication packet with map seed.
void packet_setup(uint16_t map_seed) {
    // Declare packet variable equal to given map seed.
    packet_t packet;
    packet.id = PACKET_INIT;
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
