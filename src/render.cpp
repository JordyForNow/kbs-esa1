#include "render.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Draw any cell, x -> horizontal coord, y-> verticale coord, type -> what needs to be drawn.
void draw_cell(int x, int y, cell_type_t type) {
    x = CELL_SIZE * x;
    y = CELL_SIZE * y;

    switch (type) {
        case BOMB:
            draw_rect(x, y, ILI9341_LIGHTGREY);
            draw_circle(x, y, ILI9341_OLIVE);
            break;
        case WALL:
            draw_rect(x, y, ILI9341_DARKGREY);
            break;
        case BOX:
            draw_rect(x, y, ILI9341_NAVY);
            break;
        case EMPTY:
            draw_rect(x, y, ILI9341_BLACK);
            break;
        case EXPLODING_BOMB:
            draw_rect(x, y, ILI9341_WHITE);
            break;
    }
}

void draw_rect(int x, int y, uint16_t color) {
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, color);
}

void draw_circle(int x, int y, uint16_t color) {
    x += (CELL_SIZE / 2);
    y += (CELL_SIZE / 2);

    tft.fillCircle(x, y, (CELL_SIZE / 2), color);
}

void draw_player(int x, int y, int player){
    x = (CELL_SIZE * x);
    y = (CELL_SIZE * y);

    if(player == 0){
        draw_circle(x, y, ILI9341_BLUE);
    }
    else{
        draw_circle(x, y, ILI9341_GREEN);
    }
}
