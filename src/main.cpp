#include "defines.h"
#include "game.h"
#include "render.h"
#include "segments.h"
#include "network.h"
#include "touch.h"
#include "score.h"
#include "usart.h"

void timer1_init();
void tft_brightness_init();
void adc_init();

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

    timer1_init();
    network_init();

    // Don't read the potentiometer if the dimming of the TFT is disabled.
    #if ENABLE_DIMMING_BACKLIGHT
    tft_brightness_init();
    adc_init();
    #endif /* ENABLE_DIMMING_BACKLIGHT */

    touch_init();

    // Use the screen in landscape mode.
    tft.setRotation(1);

    // The main funtion method for the touch screen.
    menus_new();

    menu_t *menu = menu_main;
    while (1) {
        // Show the menu.
        button_mode_t mode = menu_loop(menu);

        // Set up the game.
        game_init(mode);
        
        // Update the game until it ends.
        while (!game_get_state())
            if (network_update())
                game_update();
        
        if (!game_is_multiplayer()) {
            score_calculate();
        }
        
        // Clean up the game.
        game_free();

        // Initialise the menus.
        menus_new();

        // Show the correct menu depending on the game result.
        menu = game_get_state() == GAME_STATE_WON ? menu_win : menu_lose;
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

    // Compare output mode, set OC1B on Compare Match, clear at BOTTOM.
    // Only do this if dimming the TFT is enabled.
    #if ENABLE_DIMMING_BACKLIGHT
    TCCR1A |= (1 << COM1B0) | (1 << COM1B1);
    #endif /* ENABLE_DIMMING_BACKLIGHT */

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
    OCR1A = TIMER1_TOP;
    sei();
}

void tft_brightness_init(){
    DDRB |= (1 << PB2);
}

void adc_init() {
    // Define A0 as input.
    DDRC = ~(1 << PC0);

    // Use A0 as the input for the ADC.
    ADMUX = 0x00;
    
    // Turn on reference voltage.
    ADMUX |= (1 << REFS0);

    // ADCclock = CPUclock / 128.
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Enable ADC, enable auto trigger, enable ADC interrupts.
    ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE);

    // Interrupt on Timer1 compare match B.
    ADCSRB = (1 << ADTS2) | (1 << ADTS1);
    
    // Trigger first update.
    ADCSRA |= (1 << ADSC);
}

// This interrupt is thrown when the ADC is ready with the conversion.
// The value from the ADC is from 0-1023 and needs to be mapped to 0-TIMER1_TOP.
// The mapped value is put in OCR1B to set the right duty cycle so that 
// the TFT has the right brightness.
ISR(ADC_vect) {
    OCR1B = map(ADC, 0, 1023, 0, TIMER1_TOP);
}
