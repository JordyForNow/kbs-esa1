#ifndef BUFFER_H
#define BUFFER_H

#include "defines.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct buffer_t {
    uint8_t data[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} buffer_t;

void buffer_init(buffer_t *buffer);
uint8_t buffer_read(buffer_t *buffer);
void buffer_read(buffer_t *buffer, uint8_t *arr, int n);
bool buffer_write(buffer_t *buffer, uint8_t value);
bool buffer_write(buffer_t *buffer, uint8_t *arr, int n);
uint8_t buffer_available(buffer_t *buffer);
uint8_t buffer_space(buffer_t *buffer);
void buffer_clear(buffer_t *buffer);

#endif /* BUFFER_H */
