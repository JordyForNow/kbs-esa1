#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

void score_calculate();
void score_set_boxes(uint8_t boxes);
void score_calculate_position(uint8_t score_part_one, uint8_t score_part_two);
void eeprom_write_byte(uint16_t addr, uint8_t data);
uint8_t eeprom_read_byte(uint16_t addr);
void eeprom_show();

#endif /* SCORE_H */
