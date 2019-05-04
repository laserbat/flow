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

#define C1 0xe5764
#define C2 0xb4a7b
#define C3 0xe96a7
#define C4 0xb9ee9
#define C5 0x24892

#define MIN_THIRD 1.189
#define MAJ_THIRD 1.126
#define FIFTH     1.498

#define COUNT 6
#define DIV 1000

uint64_t global_t;

uint8_t saw(uint64_t t){
    return ((t & 255) + ((t + 1) & 255) + ((t + 2) & 255)) / 3;
}

uint8_t mix(uint64_t t){
    double freq, scale;
    uint64_t res = 0;
    uint64_t i, j;

    for (i = 0; i < (1<<COUNT); i ++){
        freq = 0.5;
        scale = 0.01;
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

uint8_t chord(uint64_t t){
    if ((global_t >> 18) & 1) return (
            mix(t) +
            mix(t * MAJ_THIRD + C1) +
            mix(t * FIFTH     + C2)
        ) / 3;

    return (
            mix(t) +
            mix(t * MIN_THIRD + C1) +
            mix(t * FIFTH + C2)
        ) / 3;
}

uint8_t octaves(uint64_t t){
    return (
            chord(    t * (1 + saw(global_t / 21) / 1024)) + 
            chord(2 * t * (1 + saw(global_t / 19) / 1024) + C3) + 
            chord(4 * t * (1 + saw(global_t / 18) / 768)  + C4)
        ) / 3;
}

uint8_t stereo(uint64_t t){
    uint8_t channel = t & 1;
    t >>= 1;
    
    if (channel == 0) return octaves(t);
    return octaves(t * 1.01 + C5);
}

int main(void){
    uint64_t t = 0;

    while(1){
        t += 1;
        global_t += 1;
        putchar(stereo(t));
    }
}
