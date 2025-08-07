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

#include "StrumSynth.h"

StrumSynth::StrumSynth(float sample_rate, const InstrumentState& instrument_state) 
: sample_rate{sample_rate}, instrument_state{instrument_state} {
        low_pass.Init(sample_rate);
        low_pass.SetFreq(3000.f);
        low_pass.SetRes(0.5f);

        high_pass.Init(sample_rate);
        high_pass.SetFreq(200.f);
        high_pass.SetRes(0.0f);

        filter_LFO.Init(sample_rate);
        filter_LFO.SetWaveform(Oscillator::WAVE_TRI);
        filter_LFO.SetAmp(1.0f);
        filter_LFO.SetFreq(instrument_state.bpm / 15.f); // 1/4 note triangle LFO modulates low pass filter


        // Init voices
        for (int i = 0; i < 16; i++) {
            StrumVoice new_voice;

            new_voice.osc.Init(sample_rate);
            new_voice.osc.SetWaveform(Oscillator::WAVE_TRI);
            new_voice.osc.SetAmp(0.8f);
            new_voice.osc.SetFreq(0.f);

            new_voice.env.Init(sample_rate);
            new_voice.env.SetAttackTime(0.005f);
            new_voice.env.SetDecayTime(0.2f);
            new_voice.env.SetSustainLevel(0.5f);
            new_voice.env.SetReleaseTime(0.55f);     
            
            voices.push_back(new_voice);
        }
}
    
float StrumSynth::process() {
    filter_LFO.SetFreq(instrument_state.bpm / 15.f); // Call every sample in case bpm changes
    float filter_freq = (filter_LFO.Process() * 1500.f) + 2500.f; // Vary cutoff from 2500 to 4000 Hz
    low_pass.SetFreq(filter_freq);
    
    float sum = 0.f;
    int count = 0;

    for (auto& voice : voices) {
        float s = voice.osc.Process() * voice.env.Process(voice.gate);
        voice.gate = false;

        if (std::abs(s) > 1e-5f) {
            sum += s;
            ++count;
        }
    }
    float scale = count > 0 ? 1.f / std::sqrt((float)count) : 0.f;
    float sample = sum * scale;

    low_pass.Process(sample);
    float mixed_sample = (low_pass.Low() * 0.5f) + (sample * 0.5f);
    high_pass.Process(mixed_sample);
    return high_pass.High() * 0.85f;
}

void StrumSynth::trigger_note(int arpeggio_idx) {
    voices[arpeggio_idx].env.Retrigger(0);
    voices[arpeggio_idx].gate = true;
}

void StrumSynth::set_arpeggio(std::vector<int> chord_notes) {
    // Convert chord to ordered 4 octave arpeggio
    std::vector<int> ordered_arpeggio;
    for (int i = 0; i <= 3; i++) {
        for (uint j = 0; j < chord_notes.size(); j++) {
            ordered_arpeggio.push_back(chord_notes[j] + (i*12));
        }
    }

    // Stagger the arpeggio
    std::vector<int> staggered_arpeggio;
    for (uint i = 0; i <= (ordered_arpeggio.size() - 3); i++) {
        staggered_arpeggio.push_back(ordered_arpeggio[i+1]);
        staggered_arpeggio.push_back(ordered_arpeggio[i+2]);
        staggered_arpeggio.push_back(ordered_arpeggio[i]);
    }

    // Populate the voices with first 16 arpeggio notes
    for (int i = 0; i <= 16; i++) {
        voices[i].osc.SetFreq(mtof(staggered_arpeggio[i]));
    }
}