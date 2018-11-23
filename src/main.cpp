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

    cli();
    #if DEBUG
    Serial.println("Setup timer 1" );
    #endif
    TCCR1A = (1 << WGM10) | (1 << WGM11); 
    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS12) | (0 << CS11) | (1 << CS10); // prescaler: 1024
    TIMSK1 = (1 << TOIE1); // interrupts: overflow
    OCR1A = 15625;
    sei();

    game_init();

    while (1) {
        game_update();
    }
    return 0;
}

ISR(TIMER1_OVF_vect)
{
    game_trigger_update();
}
