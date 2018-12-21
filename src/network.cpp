#include "network.h"

#include "usart.h"

void network_init() {
    usart_init();
}

bool network_update() {
    return usart_update();
}

void network_send(uint16_t data) {
    usart_send(data);
}

packet_t* network_receive() {
    return usart_receive();
}

bool network_available() {
    return usart_available();
}

void network_clear() {
    usart_clear();
}
