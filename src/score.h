#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

void score_calculate();
void score_set_boxes(uint8_t boxes);
void score_calculate_position(uint16_t score);

// Eeprom functions.
void eeprom_put(uint16_t addr, uint16_t data);
uint16_t eeprom_get(uint16_t addr);

#endif /* SCORE_H */
