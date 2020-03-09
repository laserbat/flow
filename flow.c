/*
 * Copyright © 2019 Olga Ustiuzhanina <me@laserbat.pw>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * the COPYING file for more details.
 */

#include <stdio.h>
#include <stdint.h>

#define COUNT 7
#define DIV 1000

uint8_t saw(uint64_t t){
    return t & 255;
}

uint8_t mix(uint64_t t){
    double freq, scale;
    uint64_t res = 0;
    uint64_t i, j;

    for (i = 0; i < (1<<COUNT); i ++){
        freq = 0.5;
        scale = 0.1;
        j = i;
        while (j > 0){
            freq += (j & 1) * scale;
            j >>= 1;
            scale /= DIV;
        }

        res += saw(t * freq);
    }

    return res / (1<<COUNT);
}

int main(void){
    uint64_t x = 0;

    while(1){
        x += 1;
        putchar(mix(x));
    }
}
