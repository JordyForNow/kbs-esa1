#include "touch.h"
#include "defines.h"
#include "render.h"

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

void touch_init(){
    if (!ts.begin()) {
        LOGLN("Couldn't start touchscreen");
        while (1);
    }
    LOGLN("Touchscreen started");
}

void touch_main(){
    int start_game = 0;
    draw_main_menu();

    while(!start_game){
        while(!(ts.touched())){}
        TS_Point touch_point = ts.getPoint();

        int prev_x = touch_point.x;
        touch_point.x = map(touch_point.y, TS_MINX, TS_MAXX,0, tft.width());
        touch_point.y = map(prev_x, TS_MAXY, TS_MINY, 0, tft.height());

        if(touch_point.x > 60 && touch_point.x < 260){
            if(touch_point.y > 50 && touch_point.y < 90){
                draw_play_menu();
            }
            else if(touch_point.y > 100 && touch_point.y < 140){
                draw_highscore_menu();
            }
        }

    }
}

void draw_main_menu(){
    draw_background(ILI9341_NAVY);

    tft.setCursor(80, 10);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(3);
    tft.println("BOMBERMAN");

    draw_button(1, "Play\0");
    draw_button(2, "Highscores\0");
}

void draw_play_menu(){
    draw_background(ILI9341_NAVY);

    tft.setCursor(80, 10);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(3);
    tft.println("PLAY GAME");

    draw_button(1, "singleplayer\0");
    draw_button(2, "Multiplayer\0");
}

void draw_highscore_menu(){
    draw_background(ILI9341_NAVY);

    tft.setCursor(80, 10);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(3);
    tft.println("HIGHSCORES");

    draw_button(4, "back\0");
}