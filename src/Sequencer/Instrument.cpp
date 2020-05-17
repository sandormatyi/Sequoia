#include "Sequencer/Instrument.h"
#include <cstring>

Instrument::Instrument(const char *name, Note note)
    : _note(note)
{
    strcpy(_name, name);
    clear();
}

const char *Instrument::getName() const
{
    return _name;
}

const Note &Instrument::getNote() const
{
    return _note;
}

bool Instrument::isBeatSet(uint8_t idx) const
{
    return _beats[idx];
}

void Instrument::setBeat(uint8_t idx, bool value)
{
    _beats[idx] = value;
}

void Instrument::toggleBeat(uint8_t idx)
{
    _beats[idx] = !_beats[idx];
}

void Instrument::clear()
{
    for (uint8_t i = 0; i < s_beatNumber; ++i)
        _beats[i] = false;
}