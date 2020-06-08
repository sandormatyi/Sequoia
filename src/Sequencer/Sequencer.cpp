#include "Sequencer/Sequencer.h"
#include "DBG.h"

Sequencer::Sequencer()
    : _instruments {
        Instrument("  KICK", Note(36, 127, 1)),
        Instrument(" SNARE", Note(38, 127, 2)),
        Instrument("  HATS", Note(42, 127, 3)),
        // Instrument("OHat", Note(46, 127, 10)),
        // Instrument("LTom", Note(47, 127, 10)),
        // Instrument("HTom", Note(48, 127, 10)),
        // Instrument("Clap", Note(39, 127, 10)),
        Instrument(" SYNTH", Note(36, 127, 4))
    }
{
    muteAllInstruments(false);
}

Sequencer::~Sequencer()
{
}

Instrument& Sequencer::getInstrument(uint8_t idx)
{
    return _instruments[idx];
}

uint8_t Sequencer::getCurrentInstrumentIdx() const
{
    return _currentInstrument;
}

void Sequencer::setCurrentInstrument(uint8_t idx)
{
    DBG("SEQ: Current instrument changed from %u to %u\n", _currentInstrument, idx);
    _currentInstrument = idx;
}

Instrument& Sequencer::getCurrentInstrument()
{
    return _instruments[_currentInstrument];
}

void Sequencer::clearInstruments()
{
    DBG("SEQ: Instruments cleared\n");
    for (auto& instrument : _instruments)
        instrument.clear();
}

std::vector<Note> Sequencer::getNotes(uint8_t stepIdx)
{
    std::vector<Note> result;
    for (size_t i = 0; i < _instruments.size(); ++i) {
        if (_isMuted[i])
            continue;

        if (_instruments[i].isActiveNote(stepIdx))
            result.push_back(_instruments[i].getNote(stepIdx));
    }
    return result;
}

bool Sequencer::isInstrumentMuted(uint8_t idx) const
{
    return _isMuted[idx];
}

void Sequencer::muteInstrument(uint8_t idx, bool isMuted)
{
    DBG("SEQ: Instrument %d mute state set to %d\n", idx, isMuted);
    _isMuted[idx] = isMuted;
}

void Sequencer::muteAllInstruments(bool isMuted)
{
    for (size_t i = 0; i < _instruments.size(); ++i)
        _isMuted[i] = isMuted;
}