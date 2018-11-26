#include "game.h"
#include <util/delay.h>
#include "segments.h"
#include <Arduino.h>
#include <Wire.h>
#include "nunchuck_funcs.h"
#include "defines.h"

void timer1_init();

int main() {
    init();
    Wire.begin();

    // Use pins A2 and A3 for power for the nunchuck, and then
    // initialize the connection to the device.
    nunchuck_setpowerpins();
    nunchuck_init();

    // Serial.begin() but only if DEBUG is high.
    LOG_INIT();

    timer1_init();
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

void timer1_init()
{
    cli();

    LOGLN("Setting up Timer1");

    // Set up the timer in fast pwm mode with the top at OCR1A
    TCCR1A = (1 << WGM10) | (1 << WGM11);
    TCCR1B = (1 << WGM12) | (1 << WGM13);
    // prescaler: 1024
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
    // interrupts: overflow
    TIMSK1 = (1 << TOIE1);
    // Set the OCR1A register to 15625 so the timer overflows each second (16.000.000 / 1024 = 15625)
    OCR1A = 15625;
    sei();
}
