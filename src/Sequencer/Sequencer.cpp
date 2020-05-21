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

    muteAllInstruments(false);
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

uint8_t Sequencer::getCurrentInstrumentIdx() const
{
    return _currentInstrument;
}

void Sequencer::setCurrentInstrument(uint8_t idx)
{
    _currentInstrument = idx;
}

Instrument& Sequencer::getCurrentInstrument()
{
    return *_instruments[_currentInstrument];
}

void Sequencer::clearInstruments()
{
    for (auto instrument : _instruments)
        instrument->clear();
}

std::vector<Note> Sequencer::getNotes(uint8_t beatNumber)
{
    std::vector<Note> result;
    for (size_t i = 0; i < _instruments.size(); ++i) {
        if (_isMuted[i])
            continue;

        if (_instruments[i]->isActiveNote(beatNumber))
            result.push_back(_instruments[i]->getNote(beatNumber));
    }
    return result;
}

bool Sequencer::isInstrumentMuted(uint8_t idx) const
{
    return _isMuted[idx];
}

void Sequencer::muteInstrument(uint8_t idx, bool isMuted)
{
    _isMuted[idx] = isMuted;
}

void Sequencer::muteAllInstruments(bool isMuted)
{
    for (size_t i = 0; i < _instruments.size(); ++i)
        _isMuted[i] = isMuted;
}