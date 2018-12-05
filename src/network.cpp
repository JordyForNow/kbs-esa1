#include "network.h"
#include "usart.h"

void network_init() 
{
    usart_init();
}

bool network_send(uint16_t data)
{
    return usart_send(data);
}

uint16_t network_receive()
{
    return usart_receive();
}

// TODO implement check if network has received data 
bool network_available();

void network_acknowledge()
{
    usart_acknowledge();
}