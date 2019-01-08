#ifndef INFRALED_H
#define INFRALED_H

#include <stdint.h>
#include <SoftwareSerial.h>

typedef enum {
    IR_FREQ_38K = 53,
    IR_FREQ_56K = 36,
} ir_freq_t;

void ir_init();
void ir_freq(ir_freq_t freq);

void ir_enable();
void ir_disable();

bool ir_update();
uint16_t ir_read();
void ir_write(uint16_t b);
uint8_t ir_available();
uint8_t ir_space();

void ir_clear();

#endif
