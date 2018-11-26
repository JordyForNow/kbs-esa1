#include "render.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);



void draw_rect(int x, int y, uint16_t color) {
    tft.fillRect(x, y, CELL_SIZE, CELL_SIZE, color);
}

void draw_circle(int x, int y, uint16_t color) {
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
