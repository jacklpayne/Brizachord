#pragma once
#include "daisysp.h"
#include <vector>

using namespace daisysp;

class ChordSynth
{
public:
    ChordSynth(float sample_rate) : sample_rate(sample_rate) {};

    void set_chord(std::vector<int> midi_notes);
    float process();

    void enable()  {enabled  = true;}
    void disable() {enabled  = false;}

private:
    float sample_rate;
    bool enabled;
    Oscillator bass_osc;
    std::vector<Oscillator> chord_oscs;

    Svf high_pass;
    Svf low_pass;
};
