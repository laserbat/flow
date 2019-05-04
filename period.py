#!/usr/bin/python3

#
# Copyright © 2019 Olga Ustiuzhanina <me@laserbat.pw>
#
# This program is free software. It comes without any warranty, to
# the extent permitted by applicable law. You can redistribute it
# and/or modify it under the terms of the Do What The Fuck You Want
# To Public License, Version 2, as published by Sam Hocevar. See
# the COPYING file for more details.
#

from fractions import Fraction, gcd
from math import log10

def lcm(x, y):
    return abs(x * y) / gcd(x, y)

COUNT=7
DIV=1000

result = 1
freqs = []

for i in range(0, 2**COUNT): 
    freq = Fraction(1, 2)
    scale = Fraction(1, 10)
     
    j = i
    while j > 0:
        freq += (j & 1) * scale
        j >>= 1
        scale /= DIV
    result = lcm(result, 1 / freq)

print(result * 256)
#print(10 ** (log10(DIV) * (COUNT - 1) + 1) * 256)
