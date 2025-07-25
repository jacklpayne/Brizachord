# Overview
The Brizachord is an open-source instrument and chord machine inspired by the Suzuki Omnichord. It features:
* 240 possible chords from 12 root select switches, 4 quality select switches, and 5 extension select switches
* Drum machine triggered by internal sequencer with toggleable chord pattern synchronization ("Groove"), adjustable BPM
* Velocity sensitive, multitouch capacitive strum plate for autoharp-style use
* Self-contained design with its own digital oscillators, amplification, and speaker (i.e, *not* a MIDI-only type deal).

It was designed by me and will be used in workshops held with the Audio Engineering Society at the University of Florida in Fall 2025.

# Technical
The instrument is based on the Daisy Seed audio development board and the associated `libdaisy` library for programming its STM32H7 MCU. It uses a Bela Trill Bar sensor for the strum plate and an Adafruit PAM8302A Class-D breakout for the amplifier. 

The hardware is designed for removability and reuse of the Daisy Seed and the Trill Bar.

# Future
## Software:
* Improve documentation of all functions
* USB MIDI (important)
* MIDI clock synchronization
* Data-driven CSV reading for sequencer patterns so that users can create their own more easily

## Hardware:
* Adjust Trill Bar JST cutout position
* Change corner mounting holes to M3 sized
* Find 12mm switches that are compatible with large black/white circular & square bezels to use for root selects, pattern selects
* Remove unused speaker routing holes and move Trill Bar headers to bottom of board
* Onboard power is possible, but unlikely to be implemented by me on this iteration of the board

# Environment setup
The firmware is based on a custom fork of `libdaisy` that updates the language version to `C++17` and merges @giulomoro's Trill support PR (https://github.com/electro-smith/libDaisy/pull/657). When this PR is merged upstream, the project will probably migrate to standard `libdaisy`.

# License
All software/firmware is licensed under GPLv3.
All hardware (schematic, design) is licensed under CERN-OHL-S.