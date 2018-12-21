#include "buffer.h"

void buffer_init(buffer_t *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->count = 0;
}

uint8_t buffer_read(buffer_t *buffer) {
    uint8_t element = buffer->data[buffer->tail];
    buffer->count = buffer->count - 1;
    buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
    return element;
}

void buffer_read(buffer_t *buffer, uint8_t *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = buffer_read(buffer);
    }
}

bool buffer_write(buffer_t *buffer, uint8_t value) {
    buffer->data[buffer->head] = value;
    buffer->head = (buffer->head + 1) % BUFFER_SIZE;
    buffer->count = buffer->count + 1;
    return true;
}

bool buffer_write(buffer_t *buffer, uint8_t *arr, int n) {
    for (int i = 0; i < n; i++) {
        if (!buffer_write(buffer, arr[i]))
        return false;
    }
    return true;
}

uint8_t buffer_available(buffer_t *buffer) {
    return buffer->count;
}

uint8_t buffer_space(buffer_t *buffer) {
    return BUFFER_SIZE - buffer->count;
}

void buffer_clear(buffer_t *buffer) {
    buffer->count = 0;
    buffer->head = 0;
    buffer->tail = 0;
}
