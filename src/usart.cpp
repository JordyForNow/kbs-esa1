#include "usart.h"
#include "defines.h"
#include "game.h"
#include "buff.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFFER_MAXIMUM_CAPACITY 20
#define NETWORK_ACK_BYTE 0b11000000
#define NETWORK_MAX_RETRIES 50

buffer_t * incoming_data, * outgoing_data;
volatile bool acknowledged = false;
volatile bool first_byte = true;
uint8_t currently_sending[2];
bool waiting = false;

void usart_init() {
   // enable double speed.
   UCSR0A |= (1 << U2X0);
   // Set baud rate.
   int16_t ubbr = (F_CPU / (8ul * USART_BAUD_RATE)) - 1;
   UBRR0H = (uint8_t) (ubbr / 256);
   UBRR0L = (uint8_t) (ubbr);

   // Enable Receiver and Transmitter and Reciever interrupt.
   UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

   // Create the incoming and outgoing data buffers.
   incoming_data = buffer_new(BUFFER_MAXIMUM_CAPACITY);
   outgoing_data = buffer_new(BUFFER_MAXIMUM_CAPACITY);
}

bool usart_wait_until_available() {
    while( !(UCSR0A & (1 << UDRE0)) ) { }
}


void send_bytes() {
    usart_wait_until_available();
    UDR0 = currently_sending[0];
    usart_wait_until_available();
    UDR0 = currently_sending[1];
}

bool usart_update() {
    // The game should be halted because the networking trafic has not been acknowledged yet.
    if (waiting && !acknowledged)
    {   
        return false;
    }
    
    waiting = false;
    
    if (!buffer_avail(outgoing_data))
        return true;

    // Read the ints that need to be send into the currently sending array which can be reused during retries.
    currently_sending[0] = buffer_read(outgoing_data);
    currently_sending[1] = buffer_read(outgoing_data);

    send_bytes();
    waiting = true;
    return true;
}

void usart_send(uint16_t bytes) {
    buffer_write(outgoing_data, bytes >> 8);
    buffer_write(outgoing_data, bytes);
}

int validate_incoming_data(uint16_t data) {
    // int count = 1;

    // for (int i = 0; i < 16; i++) {
    //     if (data & (1 << i)) 
    //         count++;
    // }

    // return count % 2;
    return true;
}

packet_t* usart_receive() {
    if (!buffer_avail(incoming_data))
        return NULL;

    uint16_t combined_data = (uint16_t) buffer_read(incoming_data);
    combined_data = combined_data << 8;
    combined_data |= buffer_read(incoming_data); 
    
    return validate_incoming_data(combined_data) ? packet_decode(combined_data) : NULL;
}

void usart_acknowledge() {
    usart_wait_until_available();
    UDR0 = NETWORK_ACK_BYTE;
}

bool usart_available() {
    return buffer_avail(incoming_data);
}


ISR(USART_RX_vect) {
    uint8_t temp = UDR0;
    if (first_byte && temp == NETWORK_ACK_BYTE)
    {
        acknowledged = true;
        return;
    }
    first_byte = !first_byte;
    buffer_write(incoming_data, temp);
}
