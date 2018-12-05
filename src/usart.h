#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdbool.h>

void usart_init();
bool usart_update();
void usart_send(uint16_t);
uint16_t usart_receive();
void usart_acknowledge();
bool usart_available();

#endif