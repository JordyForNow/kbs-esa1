#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdbool.h>

void usart_init();
void usart_send_bytes(uint16_t);
bool usart_has_data();
uint16_t usart_get_recieved_bytes();
void usart_send_acknowledgement();
void usart_send_debug_message(char []);

#endif