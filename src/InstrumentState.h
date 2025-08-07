#pragma once
#include <string>

enum ChordQuality {
	MAJOR, MINOR,
	AUGMENTED, DIMINISHED,
};

enum ChordExtension {
	TRIAD, SIXTH, SEVENTH,
	NINTH, ELEVENTH,
};

enum Beat {
	ROCK1, ROCK2, SLOW_ROCK, COUNTRY, SWING,
	DISCO, HIPHOP, FUNK, BOSSA, WALTZ,
};

enum Voice {
	HARP,
	ORGAN,
	WURLI,
};

struct Chord {
    std::string root;
    ChordQuality quality;
    ChordExtension extension;
};

struct InstrumentState {
    Beat beat;

    Chord chord;
	ChordQuality queued_quality;
	ChordExtension queued_extension;

    Voice chord_voice;
	
    std::string lead_note;
	Voice lead_voice;

	uint8_t bpm;
	bool groove{};
};