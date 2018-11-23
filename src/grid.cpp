#include "Grid.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

cellTypes_t grid_layout[grid_width][grid_height];  // the array where the grid items are stored

// function to initialise the twodimentional array and draw the walls
void grid_init() {
    for (int i = 0; i < grid_height; i++) {
        for (int j = 0; j < grid_width; j++) {  // make verticale walls
            if (j == 0 || j == (grid_width - 1)) {
                change_cell(j, i, wall);
            }
            if (i == 0 || i == (grid_height - 1)) {  // make horizontal walls
                change_cell(j, i, wall);
            }

            if (i > 0 && i < (grid_height - 1) && j > 0 && j < (grid_width - 1)) {  // if it isn't a sidewall
                if (i % 2 == 0 && j % 2 == 0) {                                   // make wall
                    change_cell(j, i, wall);
                } else {  // make box or make empty
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
    // clear the corners so a player can start and doesn't directly get hit by a bomb
    for (int k = 1; k < 4; k++) {
        int change_width = grid_width - 1 - k;
        int change_height = grid_height - 1 - k;

        change_cell(k, 1, empty);  // clear horizontal y=1
        change_cell(change_width, 1, empty);

        change_cell(k, (grid_height - 2), empty);  // clear horizontal y=(grid_height-1)
        change_cell(change_width, (grid_height - 2), empty);

        change_cell(1, k, empty);  // clear verticale x=1
        change_cell(1, change_height, empty);

        change_cell((grid_width - 2), k, empty);  // clear verticale x=(grid_width-1)
        change_cell((grid_width - 2), change_height, empty);
    }
}

// draw any cell, x -> horizontal coord, y-> verticale coord, type -> what needs to be drawn
void draw_cell(int x, int y, cellTypes_t type) {
    x = cell_size * x;
    y = cell_size * y;

    switch (type) {
        case player_one:
            draw_player_one(x, y);
            break;
        case player_two:
            draw_player_two(x, y);
            break;
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

//void used to change the cell. returns 1 if succeeded, 0 if failed because cell is a wall
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

void draw_player_one(int x, int y) {
    x += (cell_size / 2);
    y += (cell_size / 2);

    tft.fillCircle(x, y, (cell_size / 2), ILI9341_BLUE);
}

void draw_player_two(int x, int y) {
    x += (cell_size / 2);
    y += (cell_size / 2);
    tft.fillCircle(x, y, (cell_size / 2), ILI9341_GREEN);
}

void draw_wall(int x, int y) {
    tft.fillRect(x, y, cell_size, cell_size, ILI9341_DARKGREY);
}

void draw_bomb(int x, int y) {
    tft.fillRect(x, y, cell_size, cell_size, ILI9341_LIGHTGREY);
    x += (cell_size / 2);
    y += (cell_size / 2);
    tft.fillCircle(x, y, (cell_size / 2), ILI9341_OLIVE);
}
void draw_exploding_bomb(int x, int y) {
    tft.fillRect(x, y, cell_size, cell_size, ILI9341_WHITE);
}

void draw_box(int x, int y) {
    tft.fillRect(x, y, cell_size, cell_size, ILI9341_NAVY);
}

void draw_empty(int x, int y) {
    tft.fillRect(x, y, cell_size, cell_size, ILI9341_BLACK);
}
