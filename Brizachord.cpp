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

#include "Brizachord.h"
Brizachord* Brizachord::instance = nullptr;

Brizachord::Brizachord() {
    Brizachord::instance = this;

	// Hardware init
	hw.Configure();
	hw.Init();
	hw.SetAudioBlockSize(4);
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	// Analog inputs init
	adc_config[0].InitSingle(hw.GetPin(21)); // A0 (Strum vol)
    adc_config[1].InitSingle(hw.GetPin(22)); // A1 (Chord vol)
    adc_config[2].InitSingle(hw.GetPin(23)); // A2 (Drums vol)
    adc_config[3].InitSingle(hw.GetPin(24)); // A3 (BPM)

	// Instrument state and oscillators init
	instrument_state.chord = Chord{"C", ChordQuality::MAJOR, ChordExtension::TRIAD};
	instrument_state.queued_extension = ChordExtension::TRIAD;
	instrument_state.queued_quality = ChordQuality::MAJOR;
	instrument_state.bpm = 80;

	auto midi_notes = chord_to_midi(instrument_state.chord);
	chord_synth.set_chord(midi_notes);

	gpio_state.init();

	static Sequencer seq{48000.f, drum_synth, chord_synth};
	sequencer = &seq;

	static StrumSynth str{48000.f, instrument_state};
	strum_synth = &str;
    strum_synth->set_arpeggio(midi_notes);

	// Trill sensor setup
    int i2cBus = 1;
    trill_bar.setup(i2cBus, Trill::BAR);

	hw.StartAudio(audio_callback);
}

Brizachord::~Brizachord(){

}

void Brizachord::main_loop() {
	while(1) {
		poll_trill_bar();
		gpio_state.debounce_all();
		poll_chord_qual();
		poll_chord_ext();
		poll_chord_root();
	
	}    
}

//======================================================================
// Main callback
//======================================================================
/* 
This callback is called every sample and:
1. Ticks the sequencer forward in time
2. Collects a sample from each of the three sound sources (chord synth, strum synth, and drums)
3. Scales them in proportion to user-set volumes and adds them together
4. Sends the resulting sample to Daisy Seed's audio output pin
*/
void Brizachord::audio_callback(AudioHandle::InputBuffer  in,
                                AudioHandle::OutputBuffer out,
                                size_t                    size) {
	
									
	for (size_t i = 0; i < size; i++)
	{	
		instance->sequencer->tick();
		float sample = 0.f;
		
		float chord_sample = instance->chord_synth.process();
		float drums_sample = instance->drum_synth.process();
		float strum_sample = instance->strum_synth->process();

		sample += drums_sample * 0.2f;
		sample += chord_sample * 0.2f;
		sample += strum_sample * 0.6f;

		out[0][i] = sample;
		out[1][i] = sample;
	}
}

//======================================================================
// Polling
//======================================================================

/* Poll the chord root switches for changes. Update InstrumentState's chord root on a falling edge (press).*/
void Brizachord::poll_chord_root() {
    std::string new_root = instrument_state.chord.root;
	
	if      (gpio_state.button_C.FallingEdge())     new_root = "C";
	else if (gpio_state.button_Db.FallingEdge())    new_root = "Db";
	else if (gpio_state.button_D.FallingEdge())     new_root = "D";
	else if (gpio_state.button_Eb.FallingEdge())    new_root = "Eb";
	else if (gpio_state.button_E.FallingEdge())     new_root = "E";
	else if (gpio_state.button_F.FallingEdge())     new_root = "F";
	else if (gpio_state.button_Gb.FallingEdge())    new_root = "Gb";
	else if (gpio_state.button_G.FallingEdge())     new_root = "G";
	else if (gpio_state.button_Ab.FallingEdge())    new_root = "Ab";
	else if (gpio_state.button_A.FallingEdge())     new_root = "A";
	else if (gpio_state.button_Bb.FallingEdge())    new_root = "Bb";
	else if (gpio_state.button_B.FallingEdge())     new_root = "B";
	else                           return;

	if (new_root != instrument_state.chord.root) {
		instrument_state.chord.root = new_root;
		instrument_state.chord.quality = instrument_state.queued_quality;
		instrument_state.chord.extension = instrument_state.queued_extension;

		// Prevent audio ISR from being called while rebuilding the osc vectors
		__disable_irq();
		auto midi_notes = chord_to_midi(instrument_state.chord);
		chord_synth.set_chord(midi_notes);
		strum_synth->set_arpeggio(midi_notes);
		__enable_irq();
	}
}

/* 
Poll the chord quality switches for changes.
On falling edge (press): update the InstrumentState's queued chord quality.
On rising edge (release): update the InstrumentState's chord with its queued quality.

The queueing system allows for the user to select a new root while holding down 
the new chord quality/extension to prevent them from applying briefly to the previous chord during the change.
*/
void Brizachord::poll_chord_qual() {
    	// ---------------- Update queue ----------------------------------------------
	if (gpio_state.button_maj.FallingEdge())        instrument_state.queued_quality = MAJOR;
	else if (gpio_state.button_min.FallingEdge())   instrument_state.queued_quality = MINOR;
	else if (gpio_state.button_aug.FallingEdge())   instrument_state.queued_quality = AUGMENTED;
	else if (gpio_state.button_dim.FallingEdge())   instrument_state.queued_quality = DIMINISHED;

	// ---------------- Update chord ----------------------------------------------

	if (!(gpio_state.button_maj.RisingEdge()  || gpio_state.button_min.RisingEdge()
		|| gpio_state.button_aug.RisingEdge() || gpio_state.button_dim.RisingEdge())) {
			return;
		}

	if (instrument_state.queued_quality != instrument_state.chord.quality) {
		instrument_state.chord.quality = instrument_state.queued_quality;
		// Prevent audio ISR from being called while rebuilding the osc vectors
		__disable_irq();
		auto midi_notes = chord_to_midi(instrument_state.chord);
		chord_synth.set_chord(midi_notes);
		strum_synth->set_arpeggio(midi_notes);
		__enable_irq();
	}
}

/* 
Poll the chord extension switches for changes.
On falling edge (press): update the InstrumentState's queued chord extension.
On rising edge (release): update the InstrumentState's chord with its queued extension.

The queueing system allows for the user to select a new root while holding down 
the new chord quality/extension to prevent them from applying briefly to the previous chord during the change.
*/
void Brizachord::poll_chord_ext() {
	// ---------------- Update queue ----------------------------------------------
	if (gpio_state.button_triad.FallingEdge())      instrument_state.queued_extension = TRIAD;
	else if (gpio_state.button_7th.FallingEdge())   instrument_state.queued_extension = SEVENTH;
	else if (gpio_state.button_9th.FallingEdge())   instrument_state.queued_extension = NINTH;
	else if (gpio_state.button_11th.FallingEdge())  instrument_state.queued_extension = ELEVENTH;

	// ---------------- Update chord ----------------------------------------------
		if (!(gpio_state.button_triad.RisingEdge() 
			|| gpio_state.button_7th.RisingEdge()
			||gpio_state.button_9th.RisingEdge() 
			|| gpio_state.button_11th.RisingEdge())) {
				return;
			}

	if (instrument_state.queued_extension != instrument_state.chord.extension) {
		instrument_state.chord.extension = instrument_state.queued_extension;
		// Prevent audio ISR from being called while rebuilding the osc vectors
		__disable_irq();
		auto midi_notes = chord_to_midi(instrument_state.chord);
		chord_synth.set_chord(midi_notes);
		strum_synth->set_arpeggio(midi_notes);
		__enable_irq();
	}
}



void Brizachord::poll_pattern_controls() {
    if (gpio_state.groove.FallingEdge()) {
		sequencer->toggle_groove();
	}
	if (gpio_state.drum_left.FallingEdge()) {

	}
	if (gpio_state.drum_right.FallingEdge()) {
		
	}
}

/*
Read I2C values from Trill Bar.
Evaluate each touch and calculate correspoding indices in the strum arpeggio,
then tell them to the strum synth.
*/
void Brizachord::poll_trill_bar() {
    trill_bar.readI2C();
    if(trill_bar.getNumTouches())
    {
        for(size_t n = 0; n < trill_bar.getNumTouches(); ++n) {
            float sensor_value = trill_bar.touchLocation(n);

            // Divide and modulo the raw sensor value by the number of notes in
            // the strum synth arpeggio to get corresponding index of the note in the arpeggio
            uint8_t arpeggio_size = strum_synth->arpeggio_size();
            int arpeggio_idx = int(sensor_value * arpeggio_size) % (arpeggio_size);
            strum_synth->trigger_note(arpeggio_idx);
        }
        hw.SetLed(true);
    }
    else {
        hw.SetLed(false);
    }
}

void Brizachord::poll_pots() {
	// The pots used are linear, so this lambda applies 
	// a logarithmic conversion for the sake of audio amplitude perception
    auto log_curve = [](float x) {
        x = fclamp(x, 0.f, 1.f);
        return log10f(9.f * x + 1.f);
    };

    strum_vol = log_curve(hw.adc.GetFloat(0));
    chord_vol = log_curve(hw.adc.GetFloat(1));
    drum_vol  = log_curve(hw.adc.GetFloat(2));

    instrument_state.bpm = fmap(hw.adc.GetFloat(3), 50.f, 140.f); // linear map 0–1 to 50–140
}