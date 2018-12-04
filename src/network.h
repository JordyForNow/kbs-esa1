#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include <stdbool.h>

void network_init();
bool network_send(uint16_t);
uint16_t network_receive();
bool network_available();
void network_acknowledge();

#endif