#include "network.h"

packet_t incoming_packet;

#if USART_ENABLED

#include "usart.h"

void network_init() {
    usart_init();
}

void network_enable() {}
void network_disable() {}

bool network_update() {
    return usart_update();
}

void network_send(uint16_t data) {
    usart_send(data);
}

packet_t* network_receive() {
    packet_decode(&incoming_packet, usart_receive());
    return &incoming_packet;
}

bool network_available() {
    return usart_available();
}

void network_clear() {
    usart_clear();
}

#else

#include "infrared.h"

void network_init() {
    ir_init();
    ir_freq(IR_FREQUENCY);
}

void network_enable() {
    ir_enable();
}

void network_disable() {
    ir_disable();
}

bool network_update() {
    return ir_update();
}

void network_send(uint16_t data) {
    ir_write(data);
}

packet_t* network_receive() {
    packet_decode(&incoming_packet, ir_read());
    return &incoming_packet;
}

bool network_available() {
    return ir_available();
}

void network_clear() {
    ir_clear();
}

#endif
