#include "Grid.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

cellTypes_t gridLayout[gridWidth][gridHeight];

void grid_init(){										// function to initialise the twodimentional array and draw the walls

	for(int i = 0; i < gridHeight; i++){
		for(int j = 0; j < gridWidth; j++){					// make verticale walls
			if(j == 0 || j == (gridWidth-1)){
				gridLayout[j][i] = wall;
				drawCell(j,i,wall);
			}
			if(i == 0 || i == (gridHeight-1)){				// make horizontal walls
				gridLayout[j][i] = wall;
				drawCell(j,i,wall);
			}
		
		
			if(i > 0 && i < (gridHeight-1) && j > 0 && j < (gridWidth-1)){			// if it isn't a sidewall
				if(i % 2 == 0 && j % 2 == 0){										// make wall
					gridLayout[j][i] = wall;
					drawCell(j,i,wall);
				}
				else{																// make empty
					gridLayout[j][i] = empty;
				}
			}
		}
	}
}





void drawCell(int x, int y, cellTypes_t type){			// draw any cell, x -> horizontal coord, y-> verticale coord, type -> what needs to be drawn
	
	x = cellSize * x;
	y = cellSize * y;

	
	switch(type){
		case 0:
			drawPlayerOne(x,y);
			break;
		case 1:
			drawPlayerTwo(x,y);
			break;
		case 2:
			drawWall(x,y);
			break;
		case 3:
			drawBomb(x,y);
			break;
		case 4:
			drawBox(x,y);
			break;
		case 5:
			break;
		
	}
	
	
};


void drawPlayerOne(int x, int y){
	x += (cellSize / 2);
	y += (cellSize / 2);
	
	tft.fillCircle(x, y, (cellSize / 2), ILI9341_BLUE);
}

void drawPlayerTwo(int x, int y){
	x += (cellSize / 2);
	y += (cellSize / 2);
	tft.fillCircle(x, y, (cellSize / 2), ILI9341_GREEN);
}

void drawWall(int x, int y){
	tft.fillRect(x, y, cellSize, cellSize, ILI9341_DARKGREY);
}

void drawBomb(int x, int y){
	tft.fillRect(x, y, cellSize, cellSize, ILI9341_LIGHTGREY);
	x += (cellSize / 2);
	y += (cellSize / 2);
	tft.fillCircle(x, y, (cellSize / 2), ILI9341_OLIVE);
}

void drawBox(int x, int y){
	tft.fillRect(x, y, cellSize, cellSize, ILI9341_NAVY);
}