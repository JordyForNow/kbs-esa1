#include "render.h"
#include "defines.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Draw any cell.
void draw_cell(int x, int y, cell_type_t type) {
    x = GRID_CELL_SIZE * x;
    y = GRID_CELL_SIZE * y;

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
    tft.fillRect(x, y, GRID_CELL_SIZE, GRID_CELL_SIZE, color);
}

void draw_rect(int x, int y, int width, int height, uint16_t color){
    tft.fillRect(x, y, width, height, color);
}

void draw_circle(int x, int y, uint16_t color) {
    x += (GRID_CELL_SIZE / 2);
    y += (GRID_CELL_SIZE / 2);

    tft.fillCircle(x, y, (GRID_CELL_SIZE / 2), color);
}

void draw_background(int color){
    tft.fillScreen(color);
}

void draw_player(player_t *player) {
    draw_circle(GRID_CELL_SIZE * player->x, GRID_CELL_SIZE * player->y, ILI9341_BLUE);
}

int draw_button( int position, char *text){
    position *= 50;
    draw_rect(60,position,TOUCH_BUTTON_WIDTH, TOUCH_BUTTON_HEIGHT,ILI9341_BLUE);
    tft.setCursor(70, position+10);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(2);
    tft.println(text);
}