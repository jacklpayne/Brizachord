#pragma once
#include "daisysp.h"
using namespace daisysp;

class DrumSynth
{
public:
    DrumSynth(float sample_rate);

    void trigger_kick();
    float process_kick();
    bool kick_active() const;

    void trigger_snare();
    float process_snare();
    bool snare_active() const;

    void trigger_hat();
    float process_hat();
    bool hat_active() const;

    float process();

private:
    Oscillator kick_osc;
    AdEnv kick_amp_env;
    AdEnv kick_pitch_env;

    float sample_rate{};

    bool kick_playing{};
    float kick_duration_s{0.3f};
    float kick_timer{};
    float kick_amp{};

    Oscillator snare_osc;
    bool snare_playing{};
    float snare_duration_s{0.16f};
    float snare_timer{};
    float snare_amp{};

    Svf snare_highpass;
    Svf snare_lowpass;

    bool hat_playing{};
    float hat_duration_s{0.09f};
    float hat_timer{};
    float hat_amp{};

    Svf hat_highpass;
    Svf hat_lowpass;
};
