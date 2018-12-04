#include "usart.h"
#include "defines.h"
#include "game.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t data[10];
volatile int counter = 0;

void usart_init()
{
   /* enable double speed */
   UCSR0A |= (1 << U2X0);
   /* Set baud rate */
   int16_t ubbr = (F_CPU / (8ul * USART_BAUD_RATE)) - 1;
   UBRR0H = (uint8_t) (ubbr / 256);
   UBRR0L = (uint8_t) (ubbr);

   /* Enable Receiver and Transmitter and Reciever interrupt */
   UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
}


void usart_send_bytes(uint16_t bytes)
{
    uint8_t first = bytes >> 8;
    uint8_t second = bytes;

}

uint16_t usart_get_recieved_bytes()
{
    uint16_t combined_data = (data[0] << 8) | (data[1]); 

    return  combined_data;
}

void usart_send_acknowledgement() {
    while( !(UCSR0A & (1<<UDRE0)) ) { }

    UDR0 = '1';
}

void usart_send_debug_message(char message[]) {
    int i = 0;

    while(message[i] != 0)
    {            
        while( !(UCSR0A & (1<<UDRE0)) ) { }

        /* Put data into buffer, sends the data */
        UDR0 = message[i];
        i++;
    }
}


ISR(USART_RX_vect) {
    uint8_t temp = UDR0;
    data[counter] = temp;
    counter++;
    if (counter == 2) {
        game_trigger_network_update();
        counter = 0;
    }
}