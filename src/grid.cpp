#include "grid.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

cellTypes_t grid_layout[GRID_WIDTH][GRID_HEIGHT];  // The array where the grid items are stored.

// Function to initialise the twodimentional array and draw the walls.
void grid_init() {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {  // Make verticale walls.
            if (j == 0 || j == (GRID_WIDTH - 1)) {
                change_cell(j, i, wall);
            }
            if (i == 0 || i == (GRID_HEIGHT - 1)) {  // Make horizontal walls.
                change_cell(j, i, wall);
            }

            if (i > 0 && i < (GRID_HEIGHT - 1) && j > 0 && j < (GRID_WIDTH - 1)) {  // If it isn't a sidewall, make a sidewall.
                if (i % 2 == 0 && j % 2 == 0) {
                    change_cell(j, i, wall);
                } else {  // Make box or make empty
                    grid_layout[j][i] = empty;
                    randomSeed(TCNT1);
                    int box_bool = random(0, 2);
                    if (box_bool) {
                        change_cell(j, i, box);
                    } else {
                        grid_layout[j][i] = empty;
                    }
                }
            }
        }
    }
    // Clear the corners so a player can start and doesn't directly get hit by a bomb.
    for (int k = 1; k < 4; k++) {
        int change_width = GRID_WIDTH - 1 - k;
        int change_height = GRID_HEIGHT - 1 - k;

        change_cell(k, 1, empty);  // Clear horizontal y=1
        change_cell(change_width, 1, empty);

        change_cell(k, (GRID_HEIGHT - 2), empty);  // Clear horizontal y=(GRID_HEIGHT-2).
        change_cell(change_width, (GRID_HEIGHT - 2), empty);

        change_cell(1, k, empty);  // Clear verticale x=1.
        change_cell(1, change_height, empty);

        change_cell((GRID_WIDTH - 2), k, empty);  // Clear verticale x=(GRID_WIDTH-2).
        change_cell((GRID_WIDTH - 2), change_height, empty);
    }
}

// Draw any cell, x -> horizontal coord, y-> verticale coord, type -> what needs to be drawn.
void draw_cell(int x, int y, cellTypes_t type) {
    x = CELL_SIZE * x;
    y = CELL_SIZE * y;

    switch (type) {
        case wall:
            draw_wall(x, y);
            break;
        case bomb:
            draw_bomb(x, y);
            break;
        case box:
            draw_box(x, y);
            break;
        case empty:
            draw_empty(x, y);
            break;
        case exploding_bomb:
            draw_exploding_bomb(x, y);
            break;
    }
}

// function used to change the cell. returns 1 if succeeded, 0 if failed because cell is a wall.
int change_cell(int x, int y, cellTypes_t change_to) {
    if (grid_layout[x][y] != wall) {
        grid_layout[x][y] = change_to;
        draw_cell(x, y, change_to);
        return 1;
    }
    return 0;
}

cellTypes_t get_cell_content(int x, int y) {
    return grid_layout[x][y];
}

void draw_rect(int x, int y, uint16_t color){
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, color);
}

void draw_circle(int x, int y, uint16_t color){
    x += (CELL_SIZE / 2);
    y += (CELL_SIZE / 2);

    tft.fillCircle(x, y, (CELL_SIZE / 2), color);
}

void draw_player_one(int x, int y) {
    x += (CELL_SIZE / 2);
    y += (CELL_SIZE / 2);

    tft.fillCircle(x, y, (CELL_SIZE / 2), ILI9341_BLUE);
}

void draw_player_two(int x, int y) {
    x += (CELL_SIZE / 2);
    y += (CELL_SIZE / 2);
    tft.fillCircle(x, y, (CELL_SIZE / 2), ILI9341_GREEN);
}



void draw_wall(int x, int y) {
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, ILI9341_DARKGREY);
}

void draw_bomb(int x, int y) {
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, ILI9341_LIGHTGREY);
    x += (CELL_SIZE / 2);
    y += (CELL_SIZE / 2);
    tft.fillCircle(x, y, (CELL_SIZE / 2), ILI9341_OLIVE);
}
void draw_exploding_bomb(int x, int y) {
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, ILI9341_WHITE);
}

void draw_box(int x, int y) {
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, ILI9341_NAVY);
}

void draw_empty(int x, int y) {
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, ILI9341_BLACK);
}