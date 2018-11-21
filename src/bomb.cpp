#include "bomb.h"
#include <stdlib.h>
#include "player.h"

// Create a new bomb struct.
bomb_t *bomb_new(uint8_t x, uint8_t y) {
    bomb_t *bomb = (bomb_t*) malloc(sizeof(bomb_t));
    if (bomb == NULL)
        return bomb;

    bomb->x = x;
    bomb->y = y;
    bomb->lifeState = 0;
    return bomb;
}

// Delete a bomb struct.
void bomb_free(bomb_t *bomb) {
    if (bomb)
        free(bomb);
}

// Update a bomb.
void bomb_update(bomb_t *bomb) {
    if(bomb->x != 0){
        if(bomb->lifeState == 3){
            //explode
        }else if(bomb->lifeState == 6){
            //delete bomb
        }else{
            bomb->lifeState++;
        }
    }
    return;
}