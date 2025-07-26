#pragma once
#include "daisy_seed.h"
#include "daisysp.h"
#include "utils.h"
#include "GPIOState.h"
#include "InstrumentState.h"
#include "DrumSynth.h"
#include "Sequencer.h"
#include "ChordSynth.h"
#include "StrumSynth.h"
#include "dev/trill/Trill.h"
#include <vector>

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

class Brizachord
{
public:
    Brizachord();
    ~Brizachord();

    void main_loop();
private:
    float strum_vol{};
	float chord_vol{};
	float drum_vol{};

    // The audio_callback has to be a static function to be registered with libdaisy.
    // Since it needs to call functions of class members, they are accessed through
    // a static self reference
    static Brizachord* instance;

    DaisySeed hw;
    InstrumentState instrument_state;

    GPIOState gpio_state;
    Trill trill_bar;
    DrumSynth drum_synth{48000.f};
    ChordSynth chord_synth{48000.f};
    StrumSynth* strum_synth;
    Sequencer* sequencer;

    static void audio_callback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size);

    void poll_chord_root();
    void poll_chord_qual();
    void poll_chord_ext();
    void poll_pattern_controls();
    void poll_trill_bar();
    void poll_pots();

};
