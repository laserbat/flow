#include <stdio.h>
#include <stdint.h>

#define GLOBAL_PITCH 0.02

#define OCTAVE_DOWN  0.5
#define MIN_THIRD    1.189
#define MAJ_THIRD    1.126
#define FIFTH        1.498

#define SYNTH_COUNT    128
#define DETUNE_START   0.01
#define DETUNE_OVERALL 1000

#define STEREO_NONLIN  2

uint8_t saw(uint64_t t){
    return t & 255;
}

uint8_t mix(uint64_t t, double mul){
    double freq, scale;
    double res = 0, out = 0, sum = 0;
    double temp, coeff;

    uint16_t i, j;
    uint8_t chan = t & 1;

    t >>= 1;

    for (i = 0; i < SYNTH_COUNT; i ++){
        freq = 0.5;
        scale = DETUNE_START;

        j = i;
        while (j > 0){
            freq += (j & 1) * scale;
            j >>= 1;
            scale /= DETUNE_OVERALL;
        }

        coeff = ((double)(i + 1) / (double)(SYNTH_COUNT));

        temp = saw(mul * t * freq + res);
        res += temp;

        if (chan){
            out += temp * coeff; 
            res += STEREO_NONLIN;
        } else {
            out += temp * (1 - coeff);
        }

        sum += coeff;
    }
    
    return (uint8_t)(out / sum + 0.5);
}

uint8_t chord(uint64_t t, double mul){
    if ((t >> 20) & 1) return (
            mix(t, mul) +
            mix(t, mul * FIFTH) +
            mix(t, mul * MAJ_THIRD) +
            mix(t, mul * OCTAVE_DOWN)
        ) >> 2;

    return (
            mix(t, mul) +
            mix(t, mul * FIFTH) +
            mix(t, mul * MIN_THIRD) +
            mix(t, mul * OCTAVE_DOWN)
        ) >> 2;
}

uint8_t octaves(uint64_t t, double mul){
    return (
            chord(t, mul * OCTAVE_DOWN) +
            chord(t, mul)
        ) >> 1;
}

int main(void){
    uint64_t t = 0;

    while(1){
        t += 1;
        putchar(octaves(t, GLOBAL_PITCH));
    }
}
