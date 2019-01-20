#include <stdio.h>
#include <stdint.h>

#define C1 0xe576462a
#define C2 0xb4a7b7f2
#define C3 0xe96a7b10
#define C4 0xb9ee9bb0
#define C5 0x24892047

#define COUNT 6
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
    return (mix(t) + mix(t * 1.189 + C1) + mix(t * 1.498 + C2)) / 3;
}

uint8_t octaves(uint64_t t){
    return (chord(t) + chord(2 * t + C3) + chord(4 * t + C4)) / 3;
}

int main(void){
    uint64_t t = 0;

    while(1){
        t += 1;
        putchar(octaves(t));
        putchar(octaves(t * 1.001 + C5));
    }
}
