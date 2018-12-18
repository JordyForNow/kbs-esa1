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
void network_resend();
void network_clear();

#endif /* NETWORK_H */
