#ifndef GRID_H
#define GRID_H

#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>




#define cellSize 19				// the size of one cell  -------possible other size: 19, width: 17, height 13------- 14, 23,17
#define gridWidth 17
#define gridHeight 13

#define TFT_CS 10
#define TFT_DC 9

extern int currentcolor;		// temp variable not needed for the bomberman
extern Adafruit_ILI9341 tft;


typedef enum cellTypes{
	playerOne,
	playerTwo,
	wall,
	bomb,
	box,
	empty
	} cellTypes_t ;
	
	
void drawCell(int x, int y, cellTypes_t type);
void drawPlayerOne(int x, int y);
void drawPlayerTwo(int x, int y);
void drawWall(int x, int y);
void drawBomb(int x, int y);
void drawBox(int x, int y);

void grid_init();


#endif /* GRID_H */