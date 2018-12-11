#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdlib.h>

typedef struct buffer_t {
    uint8_t *data;
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    uint8_t capacity;
} buffer_t;

buffer_t* buffer_new(uint8_t size);
void buffer_free(buffer_t *buffer);
uint8_t buffer_read(buffer_t *buffer);
void buffer_read(buffer_t *buffer, uint8_t *arr, int n);
bool buffer_write(buffer_t *buffer, uint8_t value);
bool buffer_write(buffer_t *buffer, uint8_t *arr, int n);
uint8_t buffer_available(buffer_t *buffer);

#endif
