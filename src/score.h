#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

void score_calculate();
void score_set_box_count(uint8_t boxes);
void score_insert(uint16_t score);
float get_score();

void eeprom_put(uint16_t addr, uint16_t data);
uint16_t eeprom_get(uint16_t addr);

#endif /* SCORE_H */
