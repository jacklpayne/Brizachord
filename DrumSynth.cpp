/*
GNU General Public License v3.0
Author: Jack Payne

 _______             __                                __                                  __ 
/       \           /  |                              /  |                                /  |
BBBBBBB  |  ______  BB/  ________   ______    _______ BB |____    ______    ______    ____BB |
BB |__BB | /      \ /  |/        | /      \  /       |BB      \  /      \  /      \  /    BB |
BB    BB< /BBBBBB  |BB |BBBBBBBB/  BBBBBB  |/BBBBBBB/ BBBBBBB  |/BBBBBB  |/BBBBBB  |/BBBBBBB |
BBBBBBB  |BB |  BB/ BB |  /  BB/   /    BB |BB |      BB |  BB |BB |  BB |BB |  BB/ BB |  BB |
BB |__BB |BB |      BB | /BBBB/__ /BBBBBBB |BB \_____ BB |  BB |BB \__BB |BB |      BB \__BB |
BB    BB/ BB |      BB |/BB      |BB    BB |BB       |BB |  BB |BB    BB/ BB |      BB    BB |
BBBBBBB/  BB/       BB/ BBBBBBBB/  BBBBBBB/  BBBBBBB/ BB/   BB/  BBBBBB/  BB/        BBBBBBB/	v1.0
*/

#include "DrumSynth.h"

DrumSynth::DrumSynth(float sample_rate) {
    this->sample_rate = sample_rate;

    // Initialize the kick osc
    //===============================================
    kick_osc.Init(sample_rate);
    kick_osc.SetWaveform(Oscillator::WAVE_SIN);
    kick_osc.SetAmp(1.0f);
    kick_osc.SetFreq(100.f);

    // Initialize the snare osc and bandpass filter
    //===============================================
    snare_osc.Init(sample_rate);
    snare_osc.SetWaveform(Oscillator::WAVE_SIN);
    snare_osc.SetAmp(1.f);
    snare_osc.SetFreq(200.f);

    snare_highpass.Init(sample_rate);
    snare_highpass.SetFreq(220.f);
    snare_highpass.SetRes(0.1f);

    snare_lowpass.Init(sample_rate);
    snare_lowpass.SetFreq(4000.f);
    snare_lowpass.SetRes(0.1f);

    // Initialize the hat bandpass filter
    //===============================================
    hat_highpass.Init(sample_rate);
    hat_highpass.SetFreq(3390.f);
    hat_highpass.SetRes(0.1f); 

    hat_lowpass.Init(sample_rate);
    hat_lowpass.SetFreq(8390.f);
    hat_lowpass.SetRes(0.1f); 
}

// Kick
// =====================================================
void DrumSynth::trigger_kick() {
    kick_timer = sample_rate * kick_duration_s;
    kick_amp = 1.0f;
}

float DrumSynth::process_kick() {
    if (kick_timer > 0.f) {
        kick_timer--;
        kick_amp -= (1.f/(sample_rate * kick_duration_s));
        return kick_osc.Process() * kick_amp;
    } else {
        return 0.f;
    }
}

bool DrumSynth::kick_active() const {
    return kick_timer > 0.f;
}

// Snare
// =====================================================
void DrumSynth::trigger_snare() {
    snare_timer = sample_rate * snare_duration_s;
    snare_amp = 1.f;
}

float DrumSynth::process_snare() {
    if (snare_timer > 0.f) {
        snare_timer--;
        // Random sample [-1.f, 1.f] for white noise burst
        snare_amp -= (1.f/(sample_rate * snare_duration_s));
        float snare_sample = (2.f * static_cast<float>(rand()) / RAND_MAX - 1.f) + snare_osc.Process();

        snare_highpass.Process(snare_sample);
        float hp = snare_highpass.High();
        snare_lowpass.Process(hp);

        return snare_lowpass.Low() * snare_amp;
    } else {
        return 0.f;
    }
}

bool DrumSynth::snare_active() const {
    return snare_timer > 0.f;
}

// Hat
// =====================================================
void DrumSynth::trigger_hat() {
    hat_timer = sample_rate * hat_duration_s;
    hat_amp = 1.0f;
}

float DrumSynth::process_hat() {
    if (hat_timer > 0.f) {
        hat_timer--;
        // Random sample [-1.f, 1.f] for white noise burst
         float hat_sample = (2.f * static_cast<float>(rand()) / RAND_MAX - 1.f);

        hat_highpass.Process(hat_sample);
        float hp = hat_highpass.High();
        hat_lowpass.Process(hp);

        return hat_lowpass.Low() * hat_amp * 0.4f;
    } else {
        return 0.f;
    }
}

float DrumSynth::process() {
    float kick_sample = process_kick() * 1.5f;
    float snare_sample = process_snare();
    float hat_sample = process_hat() * 0.4f;
    return (kick_sample + snare_sample + hat_sample);
}
