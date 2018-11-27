#include "touch.h"

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

void touch_init(){
if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  Serial.println("Touchscreen started");
}