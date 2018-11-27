#include "touch.h"

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

void touch_init(){
    if (!ts.begin()) {
        LOGLN("Couldn't start touchscreen");
        while (1);
    }
    LOGLN("Touchscreen started");
}
