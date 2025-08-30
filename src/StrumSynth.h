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
        // On a chord change, we update the queued midi notes and
        // only laod it into the oscillator when the note is retriggered by a strum
        int queued_midi_note;
        Adsr env;
        bool gate;
    };
    std::vector<StrumVoice> voices;

    Svf low_pass;
    Svf high_pass;
    Oscillator filter_LFO;

    const InstrumentState& instrument_state;
};
