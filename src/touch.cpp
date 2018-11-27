#include "touch.h"
#include "defines.h"
#include "render.h"

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

void touch_init(){
    if (!ts.begin()) {
        LOGLN("Couldn't start touchscreen");
        while (1);
    }
    LOGLN("Touchscreen started");
}

void draw_menu(){
    draw_background(ILI9341_NAVY);

    tft.setCursor(80, 10);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(3);
    tft.println("BOMBERMAN");

    draw_button(1, "test123\0");
    draw_button(2, "Vrachtwagen\0");
    draw_button(3, "graafmachine\0");
    draw_button(4, "Trein!\0");
}
