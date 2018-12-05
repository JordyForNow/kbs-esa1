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


bool usart_send(uint16_t bytes)
{
    uint8_t first = bytes >> 8;
    uint8_t second = bytes;

    UDR0 = first;
    while( !(UCSR0A & (1<<UDRE0)) ) { }
    UDR0 = second;

    // TODO: implement waiting for acknowledge form receiver

    return true;
}

int validate_incoming_data(uint16_t data){
    int count = 1;

    for (int i = 0; i < 16; i++) {
        if (data & (1 << i)) 
            count++;
    }

    return count % 2;
}

uint16_t usart_receive()
{
    uint16_t combined_data = (data[0] << 8) | (data[1]); 

    return  validate_incoming_data(combined_data) ? combined_data :  NULL;
}

void usart_acknowledge() {
    while( !(UCSR0A & (1<<UDRE0)) ) { }

    UDR0 = 0b110;
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

/*ISR(USART_RX_vect) {
    uint8_t temp = UDR0;
    data[counter] = temp;
    counter++;
    if (counter == 2) {
        game_trigger_network_update();
        counter = 0;
    }
}*/