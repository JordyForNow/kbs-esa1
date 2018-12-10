#include "score.h"
#include "defines.h"
#include "game.h"
#include "player.h"
#include "touch.h"

// Calculation variables.
uint8_t total_boxes = 0;
unsigned long total_time = 0;
float boxes_per_second = 0;

// Caclulate score.
void score_calculate() {
    int score_part_one = 0;
    int score_part_two = 0;

    // Get total game time.
    total_time = *game_get_time();
    // Get lives left.
    uint8_t lives_left = game_get_main_player()->lives;

    // Convert game time to seconds and calculate boxes per second.
    total_time /= GAME_UPDATE_FREQUENCY;
    boxes_per_second = (float)total_boxes / (float)total_time;

    // Multiply by lives and get final score.
    float score = boxes_per_second * lives_left;

    // Separate score into two parts to store in eeprom.
    score_part_one = boxes_per_second;
    score_part_two = (boxes_per_second - score_part_one) * 100;

    score_calculate_position(score_part_one, score_part_two);
}

void score_set_boxes(uint8_t boxes) {
    total_boxes = boxes;
}

// Check if score should be in top 3.
void score_calculate_position(uint8_t part_one, uint8_t part_two) {
    bool written = false;
    uint8_t value_one;
    uint8_t value_two;
    uint8_t temp;
    char label[10];
    
    for (uint16_t i = 0; i < 6; i += 2) {
        // If the highscore was added to top 3.
        if (written) {
            // Make sure all scores are moved one spot.
            temp = value_one;
            value_one = eeprom_read_byte(i);
            eeprom_write_byte(i, temp);
            temp = value_two;
            value_two = eeprom_read_byte(i + 1);
            eeprom_write_byte(i + 1, temp);

            component_change_text(menu_score->components[(i + 1) / 2], (i + 1) / 2, menu_get_score(i, label));
            continue;
        }

        // If the score is elligable to be in the top 3.
        if (eeprom_read_byte(i) < part_one || (eeprom_read_byte(i) == part_one && eeprom_read_byte(i + 1) < part_two)) {
            written = true;
            value_one = eeprom_read_byte(i);
            value_two = eeprom_read_byte(i + 1);

            eeprom_write_byte(i, part_one);
            eeprom_write_byte(i + 1, part_two);

            component_change_text(menu_score->components[(i + 1) / 2], (i + 1) / 2, menu_get_score(i, label));
        }
    }
}

void eeprom_write_byte(uint16_t addr, uint8_t data) {
    // Wait untill we are allowed to write.
    while (EECR & (1 << EEPE));
    EEAR = addr;
    EEDR = data;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

uint8_t eeprom_read_byte(uint16_t addr) {
    // Wait untill we are allowed to read.
    while (EECR & (1 << EEPE));
    EEAR = addr;
    EECR |= (1 << EERE);
    return EEDR;
}
