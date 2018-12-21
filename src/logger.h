#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

void logger_init();
void debug(const char *fmt, ...);
void debug_bits(uint32_t num, uint8_t bits);

#endif /* LOGGER_H */
