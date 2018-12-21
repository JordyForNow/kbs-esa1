#include "logger.h"

#include "defines.h"

#include <stdarg.h>
#include <SoftwareSerial.h>

#define LOGGER_BAUD_RATE 115200
#define LOGGER_BUFFER_SIZE 64

SoftwareSerial serial(0, 1);
char buff[LOGGER_BUFFER_SIZE];

void logger_init() {
    #if DEBUG
    serial.begin(LOGGER_BAUD_RATE);
    #endif
}

void debug(const char *fmt, ...) {
    #if DEBUG
    char buff[LOGGER_BUFFER_SIZE];

    // Format the string.
    va_list va;
    va_start(va, fmt);
    vsnprintf(buff, LOGGER_BUFFER_SIZE - 1, fmt, va);
    va_end(va);

    serial.print(buff);
    #endif
}

void debug_bits(uint32_t num, uint8_t bits) {
    #if DEBUG
    for (int i = bits - 1; i >= 0; i--) {
        debug("%u", (num >> i) & 1);
    }
    #endif
}
