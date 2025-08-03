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
    :  bpm(105), sample_rate(sample_rate), drum_synth(drum_synth), chord_synth(chord_synth) {
        init_sequences();
    }

void Sequencer::tick() {
    int SAMPLES_IN_BAR = static_cast<int>(sample_rate * ((60.f * 4.f )/ bpm));
    int step_duration = SAMPLES_IN_BAR / 16;
    int current_step = current_tick / step_duration;

    // If at a 16th note
    if (current_tick % step_duration == 0 && current_step < 16) {
        auto seq = sequences[sequence_idx];
        if (seq.drum_sequence.kick_steps[current_step])    drum_synth.trigger_kick();
        if (seq.drum_sequence.snare_steps[current_step])   drum_synth.trigger_snare();
        if (seq.drum_sequence.hat_steps[current_step])     drum_synth.trigger_hat();

        chord_synth.enable(); 
        if (groove) {
            chord_synth.set_bass_note(seq.bass_sequence[current_step]);
            if (!seq.chord_sequence[current_step]) {
                chord_synth.disable();
            }
        }    
    }

    current_tick++;
    if (current_tick >= SAMPLES_IN_BAR)
        current_tick = 0;
}

void Sequencer::scroll_pattern(bool direction) {
    if (direction) {
        sequence_idx++;
        if (sequence_idx >= sequences.size())
            sequence_idx = 0;
    } else {
        if (sequence_idx == 0)
            sequence_idx = sequences.size() - 1;
        else
            sequence_idx--;
    }

    // Reset the current tick to the start of the new pattern
    current_tick = 0;
}

void Sequencer::init_sequences() {
    Sequence seq{};
    DrumSequence drum_seq{};

    // ROCK 1 
    drum_seq.kick_steps  = {true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, };
    drum_seq.snare_steps = {false, false, false, false, true, false, true, false, false, false, false, false, true, false, false, false, };
    drum_seq.hat_steps   = {true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, };

    seq.drum_sequence = drum_seq;
    seq.chord_sequence = {false, false, false, false, true, false, true, false, false, false, false, false, true, true, true, false};
    seq.bass_sequence = {0,0,0,0, 
        NO_BASS_FLAG, NO_BASS_FLAG, NO_BASS_FLAG, NO_BASS_FLAG,
        4, 4, 4, 4,
        NO_BASS_FLAG, NO_BASS_FLAG, 2, 2};
    sequences.push_back(seq);

    
    // LATIN
    drum_seq.kick_steps  = {true, false, false, false, true, false, false, false, true, true, false, false, true, false, false, false, };
    drum_seq.snare_steps = {false, false, false, true, false, false, true, false, false, false, false, true, false, false, true, false, };
    drum_seq.hat_steps   = {true, false, true, false, true, false, true, false, true, true, true, false, true, false, true, false, };

    seq.drum_sequence = drum_seq;
    //seq.chord_sequence = {false, false, false, true, false, false, true, true, false, false, false, true, false, false, true, true};
    //seq.chord_sequence = {false, false, false, true, false, false, true, true, false, false, false, true, false, false, true, true};
    seq.bass_sequence = {12,12, NO_BASS_FLAG, NO_BASS_FLAG, 
        12,12, NO_BASS_FLAG, NO_BASS_FLAG,
        4,4, NO_BASS_FLAG, NO_BASS_FLAG,
        5,5, NO_BASS_FLAG, NO_BASS_FLAG,};
    sequences.push_back(seq);

    // DISCO
    drum_seq.kick_steps  = {true, false, false, false, true, false, false, false, true, true, false, false, true, false, false, false};
    drum_seq.snare_steps = {false, false, false, false, true, false, true, false, false, false, false, false, true, false, false, false};
    drum_seq.hat_steps   = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

    seq.drum_sequence = drum_seq;
    seq.chord_sequence = {false, false, true, false, true, false, false, false, true, false, false, true, false, false, true, false};
    sequences.push_back(seq);

    // TANGO
    drum_seq.kick_steps  = {true, false, false, false, true, false, false, false, true, false, false, false, true, false, true, false};
    drum_seq.snare_steps = {false, false, false, false, true, false, false, false, false, false, false, false, true, false, true, false};
    drum_seq.hat_steps   = {true, false, false, false, true, false, false, false, true, false, false, false, true, false, true, false};

    seq.drum_sequence = drum_seq;
    seq.chord_sequence = {false, false, false, false, true, false, false, false, false, false, false, false, true, false, true, false};
    sequences.push_back(seq);

    // BLUES
    drum_seq.kick_steps  = {true, false, false, false, true, false, false, false, true, false, false, false, true, false, true, false};
    drum_seq.snare_steps = {false, false, false, false, true, false, false, false, false, false, false, false, true, false, false, false};
    drum_seq.hat_steps   = {true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false};

    seq.drum_sequence = drum_seq;
    seq.chord_sequence = {true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false};
    sequences.push_back(seq);
}
