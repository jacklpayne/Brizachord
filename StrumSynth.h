#pragma once
#include "daisysp.h"
#include "InstrumentState.h"
#include <vector>

using namespace daisysp;
class StrumSynth
{
public:
    StrumSynth(float sample_rate, const InstrumentState&);

    void trigger_note(int midi_note);
    void set_arpeggio(std::vector<int> chord_notes);

    float process();

    int arpeggio_size() {return voices.size();}
private:
    float sample_rate;

    struct StrumVoice {
        Oscillator osc;
        Adsr env;
        bool gate;
    };
    std::vector<StrumVoice> voices;

    Svf low_pass;
    Svf high_pass;
    Oscillator filter_LFO;

    const InstrumentState& instrument_state;
};
