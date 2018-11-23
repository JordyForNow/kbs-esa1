#include "Grid.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

cellTypes_t gridLayout[gridWidth][gridHeight]; // the array where the grid items are stored

// function to initialise the twodimentional array and draw the walls
void grid_init() {
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) { // make verticale walls
            if (j == 0 || j == (gridWidth - 1)) {
                changeCell(j, i, wall);
            }
            if (i == 0 || i == (gridHeight - 1)) { // make horizontal walls
                changeCell(j, i, wall);
            }

            if (i > 0 && i < (gridHeight - 1) && j > 0 && j < (gridWidth - 1)) { // if it isn't a sidewall
                if (i % 2 == 0 && j % 2 == 0) { // make wall
                    changeCell(j, i, wall);
                } else { // make box or make empty
                    gridLayout[j][i] = empty;
                    randomSeed(TCNT1);
                    int boxBool = random(0, 2);
                    if (boxBool) {
                        changeCell(j, i, box);
                    } else {
                        gridLayout[j][i] = empty;
                    }
                }
            }
        }
    }
    // clear the corners so a player can start and doesn't directly get hit by a bomb
    for (int k = 1; k < 4; k++) {
        int changeWidth = gridWidth - 1 - k;
        int changeHeight = gridHeight - 1 - k;

        changeCell(k, 1, empty); // clear horizontal y=1
        changeCell(changeWidth, 1, empty);

        changeCell(k, (gridHeight - 2), empty); // clear horizontal y=(gridHeight-1)
        changeCell(changeWidth, (gridHeight - 2), empty);

        changeCell(1, k, empty); // clear verticale x=1
        changeCell(1, changeHeight, empty);

        changeCell((gridWidth - 2), k, empty); // clear verticale x=(gridWidth-1)
        changeCell((gridWidth - 2), changeHeight, empty);
    }
}

// draw any cell, x -> horizontal coord, y-> verticale coord, type -> what needs to be drawn
void drawCell(int x, int y, cellTypes_t type) {

    x = cellSize * x;
    y = cellSize * y;

    switch (type) {
    case playerOne:
        drawPlayerOne(x, y);
        break;
    case playerTwo:
        drawPlayerTwo(x, y);
        break;
    case wall:
        drawWall(x, y);
        break;
    case bomb:
        drawBomb(x, y);
        break;
    case box:
        drawBox(x, y);
        break;
    case empty:
        drawEmpty(x, y);
        break;
    case explodingBomb:
        drawExplodingBomb(x, y);
        break;
    }
}

//void used to change the cell. returns 1 if succeeded, 0 if failed because cell is a wall
int changeCell(int x, int y, cellTypes_t changeTo) {
    if (gridLayout[x][y] != wall) {
        gridLayout[x][y] = changeTo;
        drawCell(x, y, changeTo);
        return 1;
    }
    return 0;
}

cellTypes_t getCellContent(int x, int y) {
    return gridLayout[x][y];
}

void drawPlayerOne(int x, int y) {
    x += (cellSize / 2);
    y += (cellSize / 2);

    tft.fillCircle(x, y, (cellSize / 2), ILI9341_BLUE);
}

void drawPlayerTwo(int x, int y) {
    x += (cellSize / 2);
    y += (cellSize / 2);
    tft.fillCircle(x, y, (cellSize / 2), ILI9341_GREEN);
}

void drawWall(int x, int y) {
    tft.fillRect(x, y, cellSize, cellSize, ILI9341_DARKGREY);
}

void drawBomb(int x, int y) {
    tft.fillRect(x, y, cellSize, cellSize, ILI9341_LIGHTGREY);
    x += (cellSize / 2);
    y += (cellSize / 2);
    tft.fillCircle(x, y, (cellSize / 2), ILI9341_OLIVE);
}
void drawExplodingBomb(int x, int y) {
    tft.fillRect(x, y, cellSize, cellSize, ILI9341_WHITE);
}

void drawBox(int x, int y) {
    tft.fillRect(x, y, cellSize, cellSize, ILI9341_NAVY);
}

void drawEmpty(int x, int y) {
    tft.fillRect(x, y, cellSize, cellSize, ILI9341_BLACK);
}
