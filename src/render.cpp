#include "render.h"
#include "defines.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Draw any tile.
void draw_tile(int x, int y, tile_t type) {
    x = WORLD_TILE_SIZE * x;
    y = WORLD_TILE_SIZE * y;

    switch (type) {
        case BOMB:
            draw_rect(x, y, ILI9341_LIGHTGREY);
            draw_circle(x, y, ILI9341_OLIVE);
            break;
        case WALL:
            draw_rect(x, y, ILI9341_DARKGREY);
            break;
        case BOX:
            draw_rect(x, y, ILI9341_BROWN);
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
    tft.fillRect(x, y, WORLD_TILE_SIZE, WORLD_TILE_SIZE, color);
}

void draw_circle(int x, int y, uint16_t color) {
    x += (WORLD_TILE_SIZE / 2);
    y += (WORLD_TILE_SIZE / 2);

    tft.fillCircle(x, y, (WORLD_TILE_SIZE / 2), color);
}

void draw_player(player_t *player) {
    if (player->hit_duration) {
        if(player->is_main){
            draw_circle(WORLD_TILE_SIZE * player->x, WORLD_TILE_SIZE * player->y, ILI9341_ORANGE);
        }else{
            draw_circle(WORLD_TILE_SIZE * player->x, WORLD_TILE_SIZE * player->y, ILI9341_CYAN);
        }
        return;
    }

    if(player->is_main){
        draw_circle(WORLD_TILE_SIZE * player->x, WORLD_TILE_SIZE * player->y, ILI9341_RED);
    }else{
        draw_circle(WORLD_TILE_SIZE * player->x, WORLD_TILE_SIZE * player->y, ILI9341_BLUE);
    }
}
