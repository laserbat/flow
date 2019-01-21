#include <stdio.h>
#include <stdint.h>

#define MIN_THIRD 1.189
#define MAJ_THIRD 1.126
#define FIFTH     1.498

#define COUNT 5
#define DIV 10000

uint8_t saw(uint64_t t){
    return t & 255;
}

uint8_t mix(uint64_t t){
    double freq, scale;
    uint64_t res = 0;
    uint64_t i, j;

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

        res += saw(t * freq + res);
    }
    
    return res / (1<<COUNT);
}

uint8_t chord(uint64_t t){
    if ((t >> 20) & 1) return (
            mix(t) +
            mix(t * MAJ_THIRD) +
            mix(t * FIFTH) +
            mix(t >> 1)
        ) >> 2;

    return (
            mix(t) +
            mix(t * MIN_THIRD) +
            mix(t * FIFTH) +
            mix(t >> 2)
        ) >> 2;
}

uint8_t octaves(uint64_t t){
    return (
            chord(t >> 1) + 
            chord(t << 0) + 
            chord(t << 1) +
            chord(t << 2)
        ) >> 2;
}

uint8_t stereo(uint64_t t){
    uint8_t channel = t & 1;
    t >>= 1;
    
    if (channel == 0) return octaves(t);
    return octaves(t * 1.01);
}

int main(void){
    uint64_t t = 0;

    while(1){
        t += 1;
        putchar(octaves(t));
    }
}
