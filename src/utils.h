#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

uint16_t has_even_parity(uint16_t x){
    unsigned int count = 0, i, b = 1;

    for(i = 0; i < 16; i++){
        if( x & (b << i) ){count++;}
    }

    if( (count % 2) ){return 0;}

    return 1;
}

#endif