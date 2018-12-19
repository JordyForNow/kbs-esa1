#include "defines.h"
#include "touch.h"
#include "game.h"
#include "player.h"
#include "score.h"

// Calculation variables.
uint8_t total_boxes = 0;
unsigned long total_time = 0;
float score = 0;

// Caclulate score.
void score_calculate() {
    float boxes_per_second = 0;
    total_time = *game_get_time();
    uint8_t lives_left = game_get_main_player()->lives;

    // Convert game time to seconds and calculate boxes per second.
    total_time /= GAME_UPDATE_FREQUENCY;
    boxes_per_second = (float)total_boxes / (float)total_time;

    // Multiply by lives and get final score.
    score = boxes_per_second * lives_left * 100;

    score_insert(score);
}

void score_set_box_count(uint8_t boxes) {
    total_boxes = boxes;
}

// Check if score should be in top 3.
void score_insert(uint16_t score) {
    bool written = false;
    uint16_t temp_score;
    uint16_t temp;

    for (int i = 0; i < 3; i++) {
        // If the highscore was added to top 3.
        if (written) {
            // Make sure all scores are moved one spot.
            temp = eeprom_get(i);
            eeprom_put(i, temp_score);
            temp_score = temp;
            continue;
        }

        // If the score is elligible to be in the top 3.
        if (eeprom_get(i) < score) {
            written = true;
            temp_score = eeprom_get(i);
            eeprom_put(i, score);
        }
    }
}

// Wait for EEPROM to be ready.
inline void eeprom_wait() {
    while (EECR & (1 << EEPE));
}

float score_get() {
    return score;
}

// Write 16 bit int to EEPROM.
void eeprom_put(uint16_t addr, uint16_t incoming_data) {
    addr *= 2;
    uint8_t data;
    for (int i = 0; i < 2; i++) {
        // Wait until we are allowed to write.
        eeprom_wait();
        data = incoming_data;
        // Set address.
        EEAR = addr + i;
        // Store data.
        EEDR = data;
        // Execute store.
        EECR |= (1 << EEMPE);
        EECR |= (1 << EEPE);
        // Prepare for next byte.
        incoming_data >>= 8;
    }
}

// Read 16 bit int from EEPROM.
uint16_t eeprom_get(uint16_t addr) {
    addr *= 2;
    uint16_t data;
    uint16_t temp_data;
    for (int i = 1; i >= 0; i--) {
        // Wait until we are allowed to read.
        eeprom_wait();
        // Set address.
        EEAR = addr + i;
        EECR |= (1 << EERE);
        // Read data.
        temp_data = EEDR;
        // Store data in variable.
        data |= (temp_data << i * 8);
    }
    return data;
}
