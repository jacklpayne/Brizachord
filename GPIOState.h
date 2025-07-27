#include "daisy_seed.h"
using namespace daisy;
using namespace daisy::seed;

struct GPIOState {
    // Chord extension
    Switch button_triad;
    Switch button_7th;
    Switch button_9th;
    Switch button_11th;
    Switch button_13th;

    // Chord quality
    Switch button_maj;
    Switch button_min;
    Switch button_aug;
    Switch button_dim;

    // Chort root
    Switch button_C;
    Switch button_Db;
    Switch button_D;
    Switch button_Eb;
    Switch button_E;
    Switch button_F;
    Switch button_Gb;
    Switch button_G;
    Switch button_Ab;
    Switch button_A;
    Switch button_Bb;
    Switch button_B;

    // Drum controls
    Switch groove;
    Switch drum_left;
    Switch drum_right;

    void init() {
            button_Db.Init(D0, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_Eb.Init(D1, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_Gb.Init(D2, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_Ab.Init(D3, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_Bb.Init(D4, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            
            button_C.Init(D5, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_D.Init(D6, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_E.Init(D7, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_F.Init(D8, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_G.Init(D9, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_A.Init(D10, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_B.Init(D13, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);


            button_maj.Init(D22, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_min.Init(D21, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_aug.Init(D20, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_dim.Init(D19, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);

            button_triad.Init   (D26, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_7th.Init     (D27, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_9th.Init     (D28, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_11th.Init    (D29, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            button_13th.Init    (D30, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);

            groove.Init         (D23, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            drum_left.Init      (D24, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
            drum_right.Init     (D25, 1.f, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, GPIO::Pull::PULLUP);
    
   }

    void debounce_all() {
        button_Db.Debounce();
        button_Eb.Debounce();
        button_Gb.Debounce();
        button_Ab.Debounce();
        button_Bb.Debounce();
        
        button_C.Debounce();
        button_D.Debounce();
        button_E.Debounce();
        button_F.Debounce();
        button_G.Debounce();
        button_A.Debounce();
        button_B.Debounce();

        button_triad.Debounce();
        button_7th.Debounce();
        button_9th.Debounce();
        button_11th.Debounce();
        button_13th.Debounce();

        button_maj.Debounce();
        button_min.Debounce();
        button_aug.Debounce();
        button_dim.Debounce();

        groove.Debounce();
        drum_left.Debounce();
        drum_right.Debounce();
    }
};