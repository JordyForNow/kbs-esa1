#include "usart.h"
#include "defines.h"
#include "game.h"
#include "buffer.h"

#define BUFFER_MAXIMUM_CAPACITY 16
#define NETWORK_ACK_BYTE 0b11000000

buffer_t * incoming_data, * outgoing_data;
volatile bool acknowledged = false;
volatile bool first_byte = true;
uint8_t currently_sending[2], currently_receiving[2];
packet_t incoming_packet;
bool waiting = false;

void usart_init() {
    // Enable double speed.
    UCSR0A |= (1 << U2X0);
    // Set baud rate.
    int16_t ubbr = (F_CPU / (8ul * USART_BAUD_RATE)) - 1;
    UBRR0H = (uint8_t) (ubbr / 256);
    UBRR0L = (uint8_t) (ubbr);

    // Enable Receiver and Transmitter.
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    #if USART_ENABLED
    // Enable receiver interupts only if the interrupt is actualy used.
    UCSR0B |= (1 << RXCIE0);
    #endif /* USART_ENABLED */

    // Create the incoming and outgoing data buffers.
    incoming_data = buffer_new(BUFFER_MAXIMUM_CAPACITY);
    outgoing_data = buffer_new(BUFFER_MAXIMUM_CAPACITY);
}

void usart_wait_until_available() {
    while (!(UCSR0A & (1 << UDRE0))) {}
}

void send_bytes() {
    for (int i = 0; i < 2; i++) {
        usart_wait_until_available();
        UDR0 = currently_sending[i];
    }
}

bool usart_update() {
    // The game should be halted because the networking trafic has not been acknowledged yet.
    if (waiting && !acknowledged)
        return false;

    waiting = false;

    if (buffer_available(outgoing_data) < 2)
        return true;

    // Read the ints that need to be send into the currently sending array which can be reused during retries.
    buffer_read(outgoing_data, currently_sending, 2);

    send_bytes();
    waiting = true;
    acknowledged = false;
    return true;
}

void usart_send(uint16_t bytes) {
    buffer_write(outgoing_data, bytes >> 8);
    buffer_write(outgoing_data, bytes);
}

int validate_incoming_data(uint16_t data) {
    int count = 1;

    for (int i = 0; i < 16; i++) {
        if (data & (1 << i))
            count++;
    }

    return count % 2;
}

packet_t* usart_receive() {
    if (buffer_available(incoming_data) < 2)
        return NULL;

    // Read two bytes into the currently_receiving array.
    buffer_read(incoming_data, currently_receiving, 2);

    // Interpret the two bytes in currently_receiving as a packet.
    packet_decode(&incoming_packet, ((((uint16_t) currently_receiving[0]) << 8) | currently_receiving[1]));

    return validate_incoming_data(packet_encode(&incoming_packet)) ? &incoming_packet : NULL;
}

void usart_acknowledge() {
    usart_wait_until_available();
    UDR0 = NETWORK_ACK_BYTE;
}

bool usart_available() {
    return buffer_available(incoming_data) >= 2;
}

void usart_clear() {
    buffer_clear(incoming_data);
    buffer_clear(outgoing_data);
}

#if USART_ENABLED
ISR(USART_RX_vect) {
    uint8_t temp = UDR0;
    if (first_byte && temp == NETWORK_ACK_BYTE){
        acknowledged = true;
        return;
    }
    first_byte = !first_byte;
    buffer_write(incoming_data, temp);

    if (!first_byte) 
        usart_acknowledge();
}
#endif /* USART_ENABLED */
