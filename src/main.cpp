#include "game.h"
#include "Grid.h"

#include <util/delay.h>
#include "segments.h"
#include <Arduino.h>
#include <Wire.h>
#include "defines.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>    // for the touchscreen


int main() {
    init();
    Wire.begin();

    #if DEBUG
    Serial.begin(9600);
    #endif

	tft.begin();
	#if DEBUG
	Serial.print("TFT started!");
	#endif

	tft.setRotation(1);				// use the screen in landscape
  	tft.fillScreen(ILI9341_BLACK);    // paint black background


    game_init();
	grid_init();

    while (1) {
        game_update();
    }
    return 0;
}
