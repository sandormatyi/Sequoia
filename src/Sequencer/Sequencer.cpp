#include "Sequencer/Sequencer.h"

Sequencer::Sequencer()
{
    _instruments = {
        new Instrument("Kick", 36),
        new Instrument("Snr ", 38),
        new Instrument("CHat", 42),
        new Instrument("OHat", 46),
        new Instrument("LTom", 47),
        new Instrument("HTom", 50),
        new Instrument("Clap", 39),
        new Instrument("Cymb", 49)
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
        if (instrument->isBeatSet(beatNumber))
            result.push_back(instrument->getNote());
    }
    return result;
}
