#include "buffer.h"

buffer_t *buffer_new(uint8_t capacity) {
    buffer_t *buffer = (buffer_t * volatile) malloc(sizeof(buffer_t));
    if (!buffer)
        return NULL;

    buffer->data = (uint8_t * volatile) malloc(capacity * sizeof(uint8_t));
    if (!buffer->data) {
        free(buffer);
        return NULL;
    }

    buffer->count = 0;
    buffer->capacity = capacity;
    buffer->head = 0;
    buffer->tail = 0;
    return buffer;
}

void buffer_free(buffer_t *buffer) {
    if (!buffer)
        return;

    if (buffer->data)
        free(buffer->data);

    free(buffer);
}

bool buffer_full(buffer_t *buffer) {
    return buffer->count == buffer->capacity;
}

bool buffer_empty(buffer_t *buffer) {
    return buffer->count == 0;
}

bool buffer_available(buffer_t *buffer) {
    return buffer->count;
}

uint8_t buffer_read(buffer_t *buffer) {
    // We don't check if we *can* read data, that is the responsibility of the caller.
    uint8_t element = buffer->data[buffer->tail];
    buffer->count = buffer->count - 1;
    buffer->tail = (buffer->tail + 1) % buffer->capacity;
    return element;
}

bool buffer_write(buffer_t *buffer, uint8_t value) {
    // We don't check if we *can* write data, that is the responsibility of the caller.
    buffer->data[buffer->head] = value;
    buffer->head = (buffer->head + 1) % buffer->capacity;
    buffer->count = buffer->count + 1;
    return true;
}
