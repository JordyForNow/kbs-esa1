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

#if DEBUG
#define LOG_INIT() Serial.begin(115200);
#define LOG(msg) Serial.print("[" __FILE__ "] " msg);
#define LOGLN(msg) Serial.println("[" __FILE__ "] " msg);
#else
#define LOG_INIT() ;
#define LOG(msg) ;
#define LOGLN(msg) ;
#endif

#endif
