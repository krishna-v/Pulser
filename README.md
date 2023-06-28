# Pulser
A lightweight Arduino library to send sequences of pulses on output pins.

This library was born from the need to send a precise timed bitstream through a 433Mhz transmitter.

After unsuccessfully trying DwaRF433 (https://github.com/fterrier/dwarf433/) and RadioHead (https://www.airspayce.com/mikem/arduino/RadioHead/) I decided to write my own super simple library.

This library is inspired by DwaRF433, but allows greater flexibility.

The havells_fan example shows how one can send individual pulses, as well as pulse sequences and mix the two as desired.
