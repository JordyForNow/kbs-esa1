#include "game.h"
#include <util/delay.h>
#include "segments.h"
#include <Arduino.h>
#include <Wire.h>

int main() {
    init();

    Serial.begin(9600);
    Wire.begin();

    while (1) {
        for (int i = 0; i < 10; i++) {
            segments_show(i);
            _delay_ms(200);
        }
    }

    // game_init();
    //
    // while (1) {
    //     game_update();
    // }
    return 0;
}
