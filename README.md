Flow is an nearly infinite procedural composition based around mixing lots of saw waves detuned in a particular manner. To check it out you can either compile and play yourself. Please use the highest level of optimization provided by your compiler. Generating music in realtime is a hard task requiring lots of CPU instructions and heavy optimization!

    gcc -Ofast flow.c -o flow  
    ./flow | aplay -r 44100

Or, you can stream it from my server. Every person streaming it hears the same part of this composition.

    curl -L laserbat.pw/flow | aplay -r 44100

If you don't have a linux machine nearby, you can just listen to a pre-recorded OGG fragment of this composition:

https://laserbat.pw/stuff/flow.ogg

With default constants, flow will keep playing for 13.2 million of years without repeating. This limitation is caused by finite size 64-bit integers.

flow2.c is slightly different version of this composition, in which synths are played in harmony.

## Flow 3 / Pulses

An extended composition based on flow/flow2 with slight nonlinear dependency between synths.

    gcc -Ofast flow3.c -o flow3 
    ./flow3 | aplay -c2 -r44100

Feel free to play around with \#define's in source code to find a variation that sounds best to you!

## Troubleshooting

If your PC is too weak to run these programs in real-time, you can speed them up by reducing accuracy of computations. In flow/flow2:

    #define COUNT 7

Replace 7 with 6 or 5. The program will use 2^COUNT synths to generate music.

In flow3:

    #define SYNTH_COUNT    128

Replace 128 with 64 or any number smaller than 128. The program will use SYNTH\_COUNT synths to generate music.
