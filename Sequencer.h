#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "DrumSynth.h"
#include "ChordSynth.h"


class Sequencer
{
public:
    Sequencer(float sample_rate, DrumSynth& drum_synth, ChordSynth& chord_synth);

    void tick();

    void scroll_pattern(bool direction); // Pattern select switches call this function with 0,1 for left, right

private:
    uint8_t bpm{80};
    float sample_rate{};
    int current_tick;

    struct DrumPatternBeat {
        std::vector<bool> kick_steps;
        std::vector<bool> hat_steps;
        std::vector<bool> snare_steps;
    };

    std::vector<DrumPatternBeat> drum_patterns;
    std::vector<std::array<bool, 16>> chord_patterns;
    std::vector<std::string> bass_patterns;

    uint8_t pattern_idx{};

    DrumSynth& drum_synth;
    ChordSynth& chord_synth;
};
