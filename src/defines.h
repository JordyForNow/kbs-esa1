#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdarg.h>

#define DEBUG 1
#define SEGMENTS_ENABLE 1

#define INPUT_UP 0
#define INPUT_RIGHT 1
#define INPUT_DOWN 2
#define INPUT_LEFT 3
#define INPUT_BUTTON_Z 4
#define INPUT_BUTTON_C 5

// Logging macros that only do something when DEBUG is enabled.
// They print messages in the following style: [src/game.cpp] updating()
// It includes the source file where the logging call originated from.
#if DEBUG
// If DEBUG is enabled, enable logging.
#define LOG_INIT() Serial.begin(115200);
#define LOG(msg) Serial.print("[" __FILE__ "] " msg);
#define LOGLN(msg) Serial.println("[" __FILE__ "] " msg);
#else
// If DEBUG is disabled, don't log anything.
#define LOG_INIT() ;
#define LOG(msg) ;
#define LOGLN(msg) ;
#endif

#endif
