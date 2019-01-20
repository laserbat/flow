Flow is an nearly infinite procedural composition based around mixing lots of saw waves detuned in a particular manner. To check it out you can either compile and play yourself:

    gcc -Ofast flow.c -o flow 
    ./flow | aplay -r 44100

Or, you can stream it from my server. Every person streaming it hears the same part of this composition.

    curl -L laserbat.pw/flow | aplay -r 44100

If you don't have a linux machine nearby, you can just listen to a pre-recorded OGG fragment of this composition:

https://laserbat.pw/stuff/flow.ogg

With default constants, flow will keep playing for 13.2 million of years without repeating. This limitation is caused by finite size 64-bit integers.
