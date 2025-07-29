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

#include "ChordSynth.h"

void ChordSynth::set_chord(std::vector<int> midi_notes) {
	chord_oscs.clear();
	for (int i = 0; i < midi_notes.size(); i++) {
		daisysp::Oscillator osc;
		osc.Init(sample_rate);
		osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
		osc.SetFreq(mtof(midi_notes[i]));
		osc.SetAmp(1.f / (midi_notes.size() + 1));

		chord_oscs.push_back(osc);
	}

	bass_osc.Init(sample_rate);
	bass_osc.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
	bass_osc.SetFreq(mtof(midi_notes[0])/4.f);
	bass_osc.SetAmp(1.f / (midi_notes.size() + 1));

	high_pass.Init(sample_rate);
	high_pass.SetFreq(250.f);
	high_pass.SetRes(0.1f);

	low_pass.Init(sample_rate);
	low_pass.SetFreq(2800.f);
	low_pass.SetRes(0.1f);
}

float ChordSynth::process() {
    if (!enabled) {return 0.f;}
    float chord_sample{};
	for (auto& osc : chord_oscs) {
		chord_sample += osc.Process();
	}
	
	float bass_sample = bass_osc.Process();
	high_pass.Process(chord_sample);
	float hp = high_pass.High();
	low_pass.Process(hp + (bass_sample * 0.8f));
	return low_pass.Low();
}
