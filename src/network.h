#ifndef NETWORK_H
#define NETWORK_H

#include "packet.h"

#include <stdint.h>
#include <stdbool.h>

void network_init();
void network_enable();
void network_disable();
bool network_update();
void network_send(uint16_t data);
packet_t* network_receive();
bool network_available();
void network_clear();

#endif /* NETWORK_H */
