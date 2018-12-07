#ifndef NETWORK_H
#define NETWORK_H
#include "packet.h"

#include <stdint.h>
#include <stdbool.h>

void network_init();
bool network_update();
void  network_send(uint16_t);
packet_t* network_receive();
bool network_available();
void network_acknowledge();

#endif /* NETWORK_H */