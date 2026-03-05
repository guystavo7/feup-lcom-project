#include "bitwise.h"
#include <stdarg.h>
#include <stdio.h>

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) { 
    return msk & ~(1 << pos); 
}

uint8_t set(uint8_t msk, int pos) {
    return msk | (1 << pos);
}

bool is_set(uint8_t msk, int pos) {
    return !((msk & (1 << pos)) == 0);
}

uint8_t lsb(uint16_t wide_msk) { 
    return (uint8_t)(wide_msk & 0xFF);
}

uint8_t msb(uint16_t wide_msk) { 
    return (uint8_t)(wide_msk >> 8);
}

uint8_t mask(int pos, ...) { 
    va_list i;
    va_start(i, pos);
    uint8_t myMsk = 0;
    int myPos = pos;
    while(myPos != MSK_END){
        myMsk |= 1 << myPos;
        myPos = va_arg (i, int);
    }
    va_end(i);
    return myMsk;
}
