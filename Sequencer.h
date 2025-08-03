#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "DrumSynth.h"
#include "ChordSynth.h"
#include "utils.h"

class Sequencer
{
public:
    Sequencer(float sample_rate, DrumSynth& drum_synth, ChordSynth& chord_synth);

    void tick();

    void scroll_pattern(bool direction); // Pattern select switches call this function with 0,1 for left, right
    void set_bpm(uint8_t new_bpm) { bpm = new_bpm; }

    inline void toggle_groove() { groove = !groove; }

private:
    void init_sequences();

    struct DrumSequence {
    std::array<bool, 16> kick_steps{};
    std::array<bool, 16> hat_steps{};
    std::array<bool, 16> snare_steps{};
    };

    /*
    Fun fact (6.2.25 6:50pm): this struct just corrupted the entire firmware
    for thirty minutes because a static container (array) was declared after
    a dynamic container (vector), destroying the memory layout and producing
    screaming feedback. Embedded is so much fun!
    */
    struct Sequence {
    DrumSequence drum_sequence{};
    std::array<bool, 16> chord_sequence{};

    //Represents scale indices from two octaves below chord synth
    std::array<uint8_t, 18> bass_sequence{};
    };

    uint8_t sequence_idx{};

    uint8_t bpm{80};
    float sample_rate{};
    int current_tick{};



    DrumSynth& drum_synth;
    ChordSynth& chord_synth;

    bool groove{};
    std::vector<Sequence> sequences{};
};
