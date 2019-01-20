Flow is an infinite procedural composition based around mixing lots of saw waves detuned in a particular manner. To check it out you can either compile and play yourself:

    gcc -Ofast flow.c -o flow 
    ./flow | aplay -r 44100

Or, you can stream it from my server. Every person streaming it hears the same part of this composition.

    curl -L laserbat.pw/flow | aplay -r 44100
