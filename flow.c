#include <stdio.h>
#include <stdint.h>

#define COUNT 8
#define DIV 1000

uint8_t saw(uint32_t t){
    return t & 255;
}

uint8_t mix(uint32_t t){
    double freq, scale;
    uint32_t res = 0;
    uint32_t i, j;

    for (i = 0; i < (1<<COUNT); i ++){
        freq = 1;
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
    uint32_t x = 0;

    while(1){
        x += 1;
        putchar(mix(x));
    }
}
