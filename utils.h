#pragma once
#include <vector>
#include <unordered_map>
#include <cmath>
#include <string>
#include "InstrumentState.h"

/*
Used by the Sequencer to signal to the ChordSynth
Not to play a bass note on a given 16th
*/
constexpr uint8_t NO_BASS_FLAG = 255;

inline int note_name_to_midi(const std::string& note) {
    static const std::unordered_map<std::string, int> fourth_octave = {
    {"C", 60}, {"Db", 61}, {"D", 62}, {"Eb", 63},
    {"E", 64}, {"F", 65}, {"Gb", 66}, {"G", 67},
    {"Ab", 68}, {"A", 69}, {"Bb", 70}, {"B", 71}
    };
    return fourth_octave.at(note);
}

inline std::vector<int> chord_to_midi(Chord chord) {
    std::vector<int> midi_notes;
    // Push root
    midi_notes.push_back(note_name_to_midi(chord.root));

    // Push third and fifth
    switch (chord.quality) {
        case MAJOR:
            midi_notes.push_back(midi_notes[0] + 4);
            midi_notes.push_back(midi_notes[0] + 7);
            break;
        case MINOR:
            midi_notes.push_back(midi_notes[0] + 3);
            midi_notes.push_back(midi_notes[0] + 7);
            break;
        case AUGMENTED:
            midi_notes.push_back(midi_notes[0] + 4);
            midi_notes.push_back(midi_notes[0] + 8);
            break;
        case DIMINISHED:
            midi_notes.push_back(midi_notes[0] + 3);
            midi_notes.push_back(midi_notes[0] + 6);
            break;
    }
    
    bool major = (chord.quality == ChordQuality::MAJOR 
        || chord.quality == ChordQuality::AUGMENTED);
    switch (chord.extension) {
        case TRIAD:
            break;
            
        case SIXTH:
            midi_notes.push_back(midi_notes[0] + 9); // major 6th
            break;

        case SEVENTH:
            major ? midi_notes.push_back(midi_notes[0] + 11) // maj7
                : midi_notes.push_back(midi_notes[0] + 10); // b7
            break;

        case NINTH:
            major ? midi_notes.push_back(midi_notes[0] + 11) // maj7
                : midi_notes.push_back(midi_notes[0] + 10); // b7
            midi_notes.push_back(midi_notes[0] + 14); // major 9th
            break;

        case ELEVENTH:
            major ? midi_notes.push_back(midi_notes[0] + 11) // maj7
                : midi_notes.push_back(midi_notes[0] + 10); // b7
            midi_notes.push_back(midi_notes[0] + 14); // major 9th
            midi_notes.push_back(midi_notes[0] + 18); // #11th
            break;
    }
    return midi_notes;
}

// Takes a note name and a semitone offset and returns the note name at that offset.
// Ex: ("C4", 4) -> "E4"
// Ex: ("G5", 7) -> "D6"
inline std::string parse_interval(std::string note, int interval) {
    static const std::vector<std::string> notes = {
        "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"
    };

    std::string base = (note.length() == 3) ? note.substr(0, 2) : note.substr(0, 1);
    int octave = std::stoi(note.substr(base.length()));

    // Find index of current note
    int base_index = 0;
    for (int i = 0; i < 12; ++i) {
        if (notes[i] == base) {
            base_index = i;
            break;
        }
    }

    int new_index = base_index + interval;
    int new_octave = octave + new_index / 12;
    new_index = (new_index % 12 + 12) % 12;

    return notes[new_index] + std::to_string(new_octave);
}

inline float note_to_freq(const std::string& note) {
    static const std::unordered_map<std::string, float> base_octave = {
        {"C", 16.35}, {"Db", 17.32}, {"D", 18.35}, {"Eb", 19.45},
        {"E", 20.60}, {"F", 21.83}, {"Gb", 23.12}, {"G", 24.50},
        {"Ab", 25.96}, {"A", 27.50}, {"Bb", 29.14}, {"B", 30.87}
    };

    std::string name = (note.length() == 3) ? note.substr(0, 2) : note.substr(0, 1);
    int octave = std::stoi(note.substr(name.length()));
    return base_octave.at(name) * std::pow(2.0f, octave);
}

inline std::vector<float> chord_to_freqs(Chord chord) {
    std::vector<std::string> notes;

    // Push root
    notes.push_back(chord.root);

    // Push third and fifth
    switch (chord.quality) {
        case MAJOR:
            notes.push_back(parse_interval(chord.root, 4));
            notes.push_back(parse_interval(chord.root, 7));
            break;
        case MINOR:
            notes.push_back(parse_interval(chord.root, 3));
            notes.push_back(parse_interval(chord.root, 7));
            break;
        case AUGMENTED:
            notes.push_back(parse_interval(chord.root, 4));
            notes.push_back(parse_interval(chord.root, 8));
            break;
        case DIMINISHED:
            notes.push_back(parse_interval(chord.root, 3));
            notes.push_back(parse_interval(chord.root, 6));
            break;
    }
    
    std::vector<float> freqs;
    for (const auto& note: notes) {
        freqs.push_back(note_to_freq(note));
    }
    return freqs;
}

inline std::array<int, 24> chord_to_scale_midi(Chord chord) {
    constexpr uint8_t OCTAVE_OFFSET = 12;
    std::array<int, 24> scale_midi;

    // 1st
    scale_midi[0] = note_name_to_midi(chord.root);
    // 2nd
    scale_midi[1] = scale_midi[0] + 2;

    // 3rd
    scale_midi[2] = scale_midi[0]
        + (chord.quality == MAJOR || chord.quality == AUGMENTED ? 4 : 3); // Maj or min 3rd

    // 4th
    scale_midi[3] = scale_midi[0]
        + (chord.extension == ELEVENTH ? 6 : 5); // 4th or #4th if (sharp) eleventh chord

    // 5th
    if (chord.quality == AUGMENTED ) {
        scale_midi[4] = scale_midi[0] + 8; // Aug 5th
    } else if (chord.quality == DIMINISHED) {
        scale_midi[4] = scale_midi[0] + 6; // Dim 5th
    } else {
        scale_midi[4] = scale_midi[0] + 7; // Perf 5th
    }

    // 6th
    scale_midi[5] = scale_midi[0] + 9; // Always maj 6th

    // 7th
    scale_midi[6] = scale_midi[0]
        + (chord.quality == MAJOR || chord.quality == AUGMENTED ? 11 : 10); // maj7 or b7

    // Double the scale and drop the whole thing an octave
    for (size_t i = 0; i < 7; ++i) {
        scale_midi[7 + i] = scale_midi[i] - OCTAVE_OFFSET;
        scale_midi[i] -= (2 * OCTAVE_OFFSET);
    }
    return scale_midi;
}