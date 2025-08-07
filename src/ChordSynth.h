#pragma once
#include "daisysp.h"
#include "InstrumentState.h"
#include "utils.h"
#include <vector>

using namespace daisysp;

class ChordSynth
{
public:
    ChordSynth(float sample_rate) : sample_rate(sample_rate) {};

    void set_chord(Chord chord);
    void set_bass_note(uint8_t scale_idx);
    float process();

    void enable()  {enabled  = true;}
    void disable() {enabled  = false;}

private:
    float sample_rate;
    bool enabled;
    Oscillator bass_osc;
    std::array<int, 24> bass_scale;
    std::vector<Oscillator> chord_oscs;

    Svf high_pass;
    Svf low_pass;
};
