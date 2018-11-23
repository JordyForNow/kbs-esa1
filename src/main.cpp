#include "game.h"
#include <util/delay.h>
#include "segments.h"
#include <Arduino.h>
#include <Wire.h>
#include "defines.h"

int main() {
    init();
    Wire.begin();

    #if DEBUG
    Serial.begin(9600);
    #endif

    game_init();

    while (1) {
        game_update();
    }
    return 0;
}
