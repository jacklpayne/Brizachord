# Converts a 16-bit stereo .wav to a .h containing PCM array and length 
# Used to load a QSPI buffer with PCM data on the Flash memory for drum samples

import wave
import struct
import sys

if len(sys.argv) != 3:
    print("Usage: python wav_to_array.py input.wav output.cpp")
    sys.exit(1)

infile = sys.argv[1]
outfile = sys.argv[2]

with wave.open(infile, "rb") as wav:
    assert wav.getsampwidth() == 2, "Only 16-bit PCM supported"
    assert wav.getnchannels() == 2, "Stereo required"
    assert wav.getcomptype() == "NONE", "Compressed WAV not supported"

    frames = wav.readframes(wav.getnframes())
    samples = struct.unpack("<" + "h" * wav.getnframes() * 2, frames)

with open(outfile, "w") as out:
    out.write("#pragma once\n")
    out.write("#include <cstdint>\n\n")
    out.write(f"const unsigned int snare_pcm_len = {len(samples)};\n")
    out.write("const int16_t snare_pcm[] = {\n")

    for i, s in enumerate(samples):
        if i % 8 == 0:
            out.write("    ")
        out.write(f"{s}, ")
        if i % 8 == 7:
            out.write("\n")

    out.write("\n};\n")
