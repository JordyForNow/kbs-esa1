#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG 1
#define SEGMENTS_ENABLE 0

// The size of one cell, possible other size: 14, width: 23, height 17.
#define GRID_CELL_SIZE 19
#define GRID_WIDTH 17
#define GRID_HEIGHT 13

// The length to clear the corner from boxes.
#define GAME_STARTING_AREA 4

// The ports which the screen is connected to.
#define TFT_CS 10
#define TFT_DC 9

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8

// This is calibration data for the raw touch data to the screen coordinates.
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#endif
