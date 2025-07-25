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

#include "Sequencer.h"

Sequencer::Sequencer(float sample_rate, DrumSynth& drum_synth, ChordSynth& chord_synth) 
    : sample_rate(sample_rate), drum_synth(drum_synth), chord_synth(chord_synth), bpm(105) {
        DrumPatternBeat pat{};
        drum_patterns.clear();
        pat.kick_steps = {  true, false, false, false, 
                            true, false, false, false, 
                            true, false, false, false, 
                            true, false, false, false, };

        pat.snare_steps = {false, false, false, false, 
                            true, false, false, false, 
                            false, false, false, false, 
                            true, false, false, false, };

        pat.hat_steps = {true, false, true, false, 
                        true, false, true, false, 
                        true, false, true, false, 
                        true, false, true, false, };
        drum_patterns.push_back(pat);

        chord_patterns.push_back({true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, });
    }

void Sequencer::tick() {
    int SAMPLES_IN_BAR = static_cast<int>(sample_rate * ((60.f * 4.f )/ bpm));
    int step_duration = SAMPLES_IN_BAR / 16;
    int current_step = current_tick / step_duration;

    // If at a 16th note
    if (current_tick % step_duration == 0 && current_step < 16) {
        if (drum_patterns[pattern_idx].kick_steps[current_step])    drum_synth.trigger_kick();
        if (drum_patterns[pattern_idx].snare_steps[current_step])   drum_synth.trigger_snare();
        if (drum_patterns[pattern_idx].hat_steps[current_step])     drum_synth.trigger_hat();

        chord_synth.disable();
        if (chord_patterns[pattern_idx][current_step])
            chord_synth.enable();
    }

    current_tick++;
    if (current_tick >= SAMPLES_IN_BAR)
        current_tick = 0;
}