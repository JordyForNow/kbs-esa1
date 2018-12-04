#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdbool.h>

void usart_init();
bool usart_send(uint16_t);
uint16_t usart_receive();
void usart_acknowledge();
void usart_send_debug_message(char []);

#endif