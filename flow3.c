#include <stdio.h>
#include <stdint.h>

#define MIN_THIRD 1.189
#define MAJ_THIRD 1.126
#define FIFTH     1.498

#define COUNT 7
#define DIV 1000

uint8_t saw(uint64_t t){
    return t & 255;
}

uint8_t mix(uint64_t t, double mul){
    double freq, scale;
    uint64_t res = 0, out = 0, temp = 0;
    uint64_t i, j;

    uint8_t chan = t & 1;
    t >>= 1;

    if(t == 0) return 0;

    for (i = 0; i < (1<<COUNT); i ++){
        freq = 0.5;
        scale = 0.01;
        j = i;
        while (j > 0){
            freq += (j & 1) * scale;
            j >>= 1;
            scale /= DIV;
        }


        temp = saw(mul * t * freq + res);
        res += temp;

        if (chan == 0){
            out += temp * ((double)i / (double)(1<<COUNT));
        } else {
            out += temp * (1 - ((double)i / (double)(1<<COUNT)));
        }

    }
    
    return out / (1<<COUNT);
}

uint8_t chord(uint64_t t, double mul){
    if ((t >> 20) & 1) return (
            mix(t, mul * 0.5) +
            mix(t, mul) +
            mix(t, mul * FIFTH) +
            mix(t, mul * MAJ_THIRD)
        ) >> 2;

    return (
            mix(t, mul * 0.5) +
            mix(t, mul) +
            mix(t, mul * FIFTH) +
            mix(t, mul * MIN_THIRD)
        ) >> 2;
}

uint8_t octaves(uint64_t t, double mul){
    return (
            chord(t, mul * 0.5) +
            chord(t, mul)
        ) >> 1;
}

int main(void){
    uint64_t t = 0;

    while(1){
        t += 1;
        putchar(octaves(t, 0.02));
    }
}
