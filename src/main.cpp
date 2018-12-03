#include "defines.h"
#include "game.h"
#include "render.h"
#include "segments.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

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
    tft.begin();

    LOGLN("TFT started!");

    // Use the screen in landscape mode and paint the background black.
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    timer1_init();
    game_init();

    while (1) {
        if(!game_is_finished()){
            game_update();
        }else{
            if(game_is_won()){
                LOGLN("Won!!");
            }else{
                LOGLN("Lost :(");
            }
            while(1){}
        }
    }
    
    return 0;
}

ISR(TIMER1_OVF_vect) {
    game_trigger_update();
}

void timer1_init() {
    cli();

    LOGLN("Setting up Timer1");

    // Set up the timer in Fast PWM mode with the top at OCR1A.
    TCCR1A = (1 << WGM10) | (1 << WGM11);
    TCCR1B = (1 << WGM12) | (1 << WGM13);
    // Prescaler: 1024
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
    // Interrupts: overflow
    TIMSK1 = (1 << TOIE1);

    // Timer1 will start counting when the init function is called.
    // This means the TCNT1 can already have a value that is greater than the
    // TOP, which is equal to the OCR1A register, which in turn will lead to freezes.
    // which means the timer will have to overflow before reaching the value of TOP.
    // To prevent TCNT1 from having to overflow, we reset it to zero, therefore
    // ensuring that it does not have to overflow first.
    TCNT1 = 0;

    // Set the OCR1A register to 15625 divided by the desired update frequency
    // so the timer will generate a signal that is equal to the desired frequency.
    // (15625 is used because this will make the timer generate a signal with a frequency of 1 Hz.)
    // This number is lastly divided by the GAME_INPUT_FACTOR. We check the input
    // that many times before actually performing a game update.
    OCR1A = 15625 / GAME_UPDATE_FREQUENCY / GAME_INPUT_FACTOR;
    sei();
}
