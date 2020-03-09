// This file produces 44100 Hz, 2 channel, unsigned 8 bit raw PCM
// If you're on linux you can play it using aplay:
// $ ./flow5 | aplay -r44100 -c2

#include <stdio.h>
#include <stdint.h>

// I highly recommend playing with these constants as different values
// produces different timbres and different musical development.

// Be warned that high SYNTH_COUNT can make this program very slow.

#define STEP_COUNT     8
#define SYNTH_COUNT    160

#define GLOBAL_PITCH   0.01
#define DETUNE_START   0.0001
#define DETUNE_STEP    5
#define FREQ_START     1

#define SHIFT_MUL      10

#define LEFT_NONLIN    2
#define RIGHT_NONLIN  -3

#define MIN_THIRD      1.189207115002721
#define MAJ_THIRD      1.259921049894873

double table[SYNTH_COUNT];

uint8_t saw(uint64_t t){
    return t & 255;
}

void init_table(void) {
    uint16_t i, j;

    for (i = 0; i < SYNTH_COUNT; i ++){
        double freq, scale;
        freq = FREQ_START;
        scale = DETUNE_START;

        j = i;
        while (j > 0){
            freq += (j & 1) * scale;
            j >>= 1;
            scale /= DETUNE_STEP;
        }

        table[i] = freq;
    }
}

uint8_t mix(uint64_t t, double mul){
    double freq;
    double shift = 0, out = 0, max = 0;
    double val, amp;

    uint16_t i;
    uint8_t chan = t & 1;

    t >>= 1;

    for (i = 0; i < SYNTH_COUNT; i ++){
        freq = table[i];
        amp = ((double)(i + 1) / (double)(SYNTH_COUNT));
        val = saw(0.5 + mul * freq * t + shift);

        shift = val * SHIFT_MUL;
        max += amp;

        out += val * (chan ? amp : 1 - amp);
        shift += chan ? LEFT_NONLIN : RIGHT_NONLIN;
    }

    return (uint8_t)(out / max + 0.5);
}

uint8_t chord(uint64_t t, double mul){
    uint32_t i;
    double out = 0;

    for(i = 0; i < STEP_COUNT / 2; i ++) {
        out += mix(t, mul);
        mul *= MAJ_THIRD;
        out += mix(t, mul);
        mul *= MIN_THIRD;
    }

    out /= STEP_COUNT;

    return (uint8_t) out;
}

int main(void){
    uint64_t t = 0;

    init_table();

    while(1){
        t += 1;
        putchar(chord(t, GLOBAL_PITCH));
    }
}
