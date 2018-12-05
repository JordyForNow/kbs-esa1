#include "buff.h"
#include "usart.h"

buffer_t *buffer_new(uint8_t capacity) {
    buffer_t *buffer = (buffer_t * volatile) malloc(sizeof(buffer_t));
    
    if (!buffer)
        return NULL;
    
    buffer->buffer = (uint8_t* volatile) malloc(capacity * sizeof( uint8_t));

    if (!buffer->buffer)
        return NULL;

    buffer->count = 0;
    buffer->capacity = capacity;
    buffer->head = 0;
    buffer->tail = 0;

    return buffer;
}

void buffer_free(buffer_t *buffer) {
    if (!buffer)
        return;

    free((void *)buffer);

    if (!buffer->buffer)
        return;

    free((void *)buffer->buffer);
}

bool buffer_full(buffer_t *buffer) {
    return buffer->count == buffer->capacity;
}

bool buffer_empty(buffer_t *buffer) {
    return buffer->count == 0;
}

bool buffer_avail(buffer_t *buffer) {
    return buffer->count > 1;
}

uint8_t buffer_read(buffer_t *buffer) {
    
    if (buffer_empty(buffer))
        return NULL;

    uint8_t element = buffer->buffer[buffer->tail];
    buffer->count = buffer->count - 1;
    buffer->tail = (buffer->tail + 1) % buffer->capacity;
    return element;
}

bool buffer_write(buffer_t *buffer, uint8_t value) {
    buffer->head = (buffer->head + 1) % buffer->capacity;
    buffer->count = buffer->count + 1;

    buffer->buffer[buffer->head] = value;
    
    return true;
}