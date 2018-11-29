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

    touch_menu_page_t page_main_menu = {};
    touch_menu_page_t page_select_players = {};
    touch_menu_page_t pages_highscores {};

    touch_button_t button_main_menu_play = {};
    touch_button_t button_main_menu_highscores = {};
    touch_button_t button_back = {};
    touch_button_t button_play_singleplayer = {};
    touch_button_t button_play_multiplayer = {};

    page_main_menu.title = strdup("BOMBERMAN\0");
    page_main_menu.buttons[0] = &button_main_menu_play;
    page_main_menu.buttons[1] = &button_main_menu_highscores;

    page_select_players.title = strdup("play game\0");
    page_select_players.buttons[0] = &button_play_singleplayer;
    page_select_players.buttons[1] = &button_play_multiplayer;
    page_select_players.buttons[3] = &button_back;

    pages_highscores.title = strdup("Highscores\0");
    pages_highscores.buttons[3] = &button_back;

    button_main_menu_play.text = strdup("play\0");
    button_main_menu_play.position = 1;
    button_main_menu_play.target_page = &page_select_players;

    button_main_menu_highscores.text = strdup("Highscores\0");
    button_main_menu_highscores.position = 2;
    button_main_menu_highscores.target_page = &pages_highscores;

    button_back.text = strdup("back");
    button_back.position = 4;
    button_back.target_page = &page_main_menu;

    button_play_singleplayer.text = strdup("Singleplayer\0");
    button_play_singleplayer.position = 1;

    button_play_multiplayer.text = strdup("Multiplayer\0");
    button_play_multiplayer.position = 2;

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
    Serial.print("current page: ");
    Serial.println(current_page->title);

    while(!start_game){
        while(!(ts.touched())){}
        
        TS_Point touch_point = ts.getPoint();

        int prev_x = touch_point.x;
        touch_point.x = map(touch_point.y, TS_MINX, TS_MAXX,0, tft.width());
        touch_point.y = map(prev_x, TS_MAXY, TS_MINY, 0, tft.height());

        if(touch_point.x > TOUCH_BUTTON_START_X && touch_point.x < (TOUCH_BUTTON_START_X + TOUCH_BUTTON_WIDTH)){
            for(int i = 1; i < 5; i++){
                if(current_page->buttons[i-1]){
                    if(touch_point.y > (i * (TOUCH_BUTTON_HEIGHT + TOUCH_BUTTON_PADDING)) && touch_point.y < ((i+1) * TOUCH_BUTTON_HEIGHT + i * TOUCH_BUTTON_PADDING)){
                        Serial.print("Button pressed: ");
                        Serial.println(i-1);

                            Serial.print("direction page title: ");
                            Serial.println(current_page->buttons[i-1]->target_page->title);
                        if(current_page->buttons[i-1]->target_page){
                            touch_draw_page(current_page->buttons[i-1]->target_page);
                        } else{
                            Serial.println("no target page available");
                        }
                    }
                }
            }
        }
    }
}

//TODO: add function methods to free (strdup)
