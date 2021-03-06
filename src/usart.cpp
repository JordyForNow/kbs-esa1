#include "usart.h"

#include "buffer.h"
#include "defines.h"
#include "game.h"

#define BUFFER_MAXIMUM_CAPACITY 16
#define NETWORK_ACK_BYTE 0b11000000

buffer_t incoming_data, outgoing_data;
volatile bool acknowledged = true;
volatile bool first_byte = true;
uint8_t currently_sending[2], currently_receiving[2];

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
    buffer_init(&incoming_data);
    buffer_init(&outgoing_data);
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
    if (!acknowledged)
        return false;

    if (buffer_available(&outgoing_data) < 2)
        return true;

    // Read the ints that need to be send into the currently sending array so they can be reused during retries.
    buffer_read(&outgoing_data, currently_sending, 2);

    send_bytes();
    acknowledged = false;
    return true;
}

void usart_send(uint16_t bytes) {
    buffer_write(&outgoing_data, bytes >> 8);
    buffer_write(&outgoing_data, bytes);
}

uint16_t usart_receive() {
    if (buffer_available(&incoming_data) < 2)
        return NULL;

    // Read two bytes into the currently_receiving array.
    buffer_read(&incoming_data, currently_receiving, 2);

    // Interpret the two bytes in currently_receiving as a uint16_t.
    uint16_t data = ((((uint16_t) currently_receiving[0]) << 8) | currently_receiving[1]);

    return has_even_parity(data) ? data : NULL;
}

void usart_acknowledge() {
    usart_wait_until_available();
    UDR0 = NETWORK_ACK_BYTE;
}

bool usart_available() {
    return buffer_available(&incoming_data) >= 2;
}

void usart_clear() {
    buffer_init(&incoming_data);
    buffer_init(&outgoing_data);
}

#if USART_ENABLED
ISR(USART_RX_vect) {
    uint8_t incoming_byte = UDR0;
    if (first_byte && incoming_byte == NETWORK_ACK_BYTE) {
        acknowledged = true;
        return;
    }
    first_byte = !first_byte;
    buffer_write(&incoming_data, incoming_byte);

    if (!first_byte)
        usart_acknowledge();
}
#endif /* USART_ENABLED */
