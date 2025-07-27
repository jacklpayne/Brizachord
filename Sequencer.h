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
    void set_bpm(uint8_t new_bpm) { bpm = new_bpm; }

    inline void toggle_groove() { groove = !groove; }

private:
    uint8_t bpm{80};
    float sample_rate{};
    int current_tick;

    struct DrumSequence {
        std::vector<bool> kick_steps;
        std::vector<bool> hat_steps;
        std::vector<bool> snare_steps;
    };

    struct Sequence {
        DrumSequence drum_sequence;
        std::array<bool, 16> chord_sequence;
    };

    std::vector<Sequence> sequences;
    uint8_t sequence_idx{};

    void init_sequences();

    DrumSynth& drum_synth;
    ChordSynth& chord_synth;

    bool groove{};
};
