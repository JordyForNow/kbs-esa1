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

void create_touch_pages(){

    touch_menu_page_t page_select_players;
    touch_menu_page_t pages_highscores;

    touch_button_t button_main_menu_play = {.text = "Play\0", .position = 1};
    touch_button_t button_main_menu_highscores = {.text = "Highscores\0", .position = 2};
    touch_menu_page_t page_main_menu = {.title = "BOMBERMAN\0"};
    page_main_menu.buttons[0] = &button_main_menu_play;
    page_main_menu.buttons[1] = &button_main_menu_highscores;

    touch_button_t button_back = {.text = "back\0", .position = 4, .target_page = &page_main_menu};

    touch_button_t button_play_singleplayer = {.text = "singleplayer\0", .position = 1};
    touch_button_t button_play_multiplayer = {.text = "multiplayer\0", .position = 2};

    page_select_players.title = "play game\0";
    page_select_players.buttons[0] = &button_play_singleplayer;
    page_select_players.buttons[1] = &button_play_multiplayer;
    page_select_players.buttons[3] = &button_back;
    
    pages_highscores.title = "Highscores\0";
    pages_highscores.buttons[3] = &button_back;

    touch_draw_page(&page_main_menu);
}

void touch_draw_page(touch_menu_page_t *page){
    draw_background(ILI9341_NAVY);
    tft.setCursor(80, 10);
    tft.setTextColor(ILI9341_WHITE);  
    tft.setTextSize(3);
    tft.println(page->title);

    if(page->buttons[0] >0) {
        draw_button(page->buttons[0]->position, page->buttons[0]->text);
    }
    if(page->buttons[1] > 0) {
        draw_button(page->buttons[1]->position, page->buttons[1]->text);
    }
    if(page->buttons[2] > 0) {
        draw_button(page->buttons[2]->position, page->buttons[2]->text);
    }
    if(page->buttons[3] > 0) {
        draw_button(page->buttons[3]->position, page->buttons[3]->text);
    }

    touch_handler(page);
}

void touch_handler(touch_menu_page_t *current_page){
    int start_game = 0;

    while(!start_game){
        while(!(ts.touched())){}
        
        Serial.println("TS touched!");
        TS_Point touch_point = ts.getPoint();

        int prev_x = touch_point.x;
        touch_point.x = map(touch_point.y, TS_MINX, TS_MAXX,0, tft.width());
        touch_point.y = map(prev_x, TS_MAXY, TS_MINY, 0, tft.height());

        Serial.print("x: ");
        Serial.println(touch_point.x);
        Serial.print("y: ");
        Serial.println(touch_point.y);

        if(touch_point.x > TOUCH_BUTTON_START_X && touch_point.x < (TOUCH_BUTTON_START_X + TOUCH_BUTTON_WIDTH)){
            Serial.println("right horizontal area");
            int j;
            for(int i = 1; i < 5; i++){
                j = i;
                Serial.print("min: ");
                Serial.println(i * TOUCH_BUTTON_HEIGHT + TOUCH_BUTTON_PADDING);
                Serial.print("max: ");
                Serial.println(i);
            }
        }
    }
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

        if(touch_point.x > TOUCH_BUTTON_START_X && touch_point.x < (TOUCH_BUTTON_START_X + TOUCH_BUTTON_WIDTH)){
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

