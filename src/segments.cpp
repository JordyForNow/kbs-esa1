#include "segments.h"

#include "defines.h"

static uint8_t segments_numbers[] = {
    0b10001000, // 0
	0b11101011, // 1
	0b01001100, // 2
	0b01001001, // 3
	0b00101011, // 4
	0b00011001, // 5
	0b00011000, // 6
	0b11001011, // 7
	0b00001000, // 8
	0b00001001, // 9
};

void segments_show(uint8_t num) {
    #if SEGMENTS_ENABLE
    Wire.beginTransmission(56);
    Wire.write((unsigned int) segments_numbers[num]);
    Wire.endTransmission();
    #endif
}

void segments_hide() {
    #if SEGMENTS_ENABLE
    Wire.beginTransmission(56);
    Wire.write((unsigned int) 0xFF);
    Wire.endTransmission();
    #endif
}
