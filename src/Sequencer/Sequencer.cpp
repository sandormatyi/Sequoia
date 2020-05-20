#include "Sequencer/Sequencer.h"

Sequencer::Sequencer()
{
    _instruments = {
        new Instrument("Kick", Note(36, 127, 10)),
        new Instrument("Snr ", Note(38, 127, 10)),
        new Instrument("CHat", Note(42, 127, 10)),
        new Instrument("OHat", Note(46, 127, 10)),
        new Instrument("LTom", Note(47, 127, 10)),
        new Instrument("HTom", Note(48, 127, 10)),
        new Instrument("Clap", Note(39, 127, 10)),
        new Instrument("ACID", Note(36, 127, 1))
    };
}

Sequencer::~Sequencer()
{
    for (auto i : _instruments)
        delete i;
}

Instrument& Sequencer::getInstrument(uint8_t idx)
{
    return *_instruments[idx];
}

void Sequencer::clearInstruments()
{
    for (auto instrument : _instruments)
        instrument->clear();
}

std::vector<Note> Sequencer::getNotes(uint8_t beatNumber)
{
    std::vector<Note> result;
    for (auto instrument : _instruments) {
        if (instrument->isActiveNote(beatNumber))
            result.push_back(instrument->getNote(beatNumber));
    }
    return result;
}
