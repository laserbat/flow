#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_SYNTHS 2048

#define OCTAVE_DOWN    0.5
#define MIN_THIRD      1.2
#define MAJ_THIRD      1.125
#define P_FIFTH        1.5

uint16_t synth_count = 128;

double global_pitch  =  0.005;
double detune_start  =  0.001;
double detune_step   =  100;
double freq_start    =  0.8;

int16_t left_nonlin  =  2;
int16_t right_nonlin = -3;


double table[MAX_SYNTHS];

uint8_t saw(uint64_t t){
    return t & 255;
}

void init_table(void) {
    uint16_t i, j;

    for (i = 0; i < synth_count; i ++){
        double freq, scale;
        freq = freq_start;
        scale = detune_start;

        j = i;
        while (j > 0){
            freq += (j & 1) * scale;
            j >>= 1;
            scale /= detune_step;
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

    for (i = 0; i < synth_count; i ++){
        freq = table[i];
        amp = ((double)(i + 1) / (double)(synth_count));
        val = saw(0.5 + mul * freq * t + shift);

        shift += val;
        max += amp;

        out += val * (chan ? amp : 1 - amp);
        shift += chan ? left_nonlin : right_nonlin;
    }

    return (uint8_t)(out / max + 0.5);
}

uint8_t chord(uint64_t t, double mul){
    return (
            mix(t, mul) +
            mix(t, mul * P_FIFTH) +
            mix(t, mul * (((t >> 20) & 1) ? MAJ_THIRD : MIN_THIRD)) +
            mix(t, mul * OCTAVE_DOWN)
        ) >> 2;
}

uint8_t octaves(uint64_t t, double mul){
    return (
            chord(t, mul * OCTAVE_DOWN) +
            chord(t, mul)
        ) >> 1;
}

int help(void){
    printf("Available parameters:\n"
            "\t-c VAL\t\tnumber of synths (int)\n"
            "\t-l VAL\t\tleft channel feedback (int)\n"
            "\t-r VAL\t\tright channel feedback (int)\n"
            "\t-f VAL\t\tstarting frequency (double)\n"
            "\t-g VAL\t\tglobal pitch multiplier (double)\n"
            "\t-d VAL\t\tstarting detune (double)\n"
            "\t-s VAL\t\tdetune step (double)\n"
            "\t-S VAL\t\tnumber of samples to skip (long long int)\n"
            );
    return -1;
}

int main(int argc, char *argv[]){
    uint64_t t = 0;
    int16_t i, j;

    for (i = 1; i < argc; i ++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    help();
                    return 0;
                case 'c':
                    i += 1;
                    synth_count = atoi(argv[i]);
                    break;
                case 'l':
                    i += 1;
                    left_nonlin = atoi(argv[i]);
                    break;
                case 'r':
                    i += 1;
                    right_nonlin = atoi(argv[i]);
                    break;
                case 'f':
                    i += 1;
                    freq_start = atof(argv[i]);
                    break;
                case 'g':
                    i += 1;
                    global_pitch = atof(argv[i]);
                    break;
                case 'd':
                    i += 1;
                    detune_start = atof(argv[i]);
                    break;
                case 's':
                    i += 1;
                    detune_step = atof(argv[i]);
                    break;
                case 'S':
                    i += 1;
                    t += atoll(argv[i]);
                    break;
                default:
                    return help();
            }
        } else return help();
    }

    init_table();

    while(1){
        t += 1;
        putchar(octaves(t, global_pitch));
    }
}
