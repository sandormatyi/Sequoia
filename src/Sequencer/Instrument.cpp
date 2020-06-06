#include "Sequencer/Instrument.h"
#include <cstring>

Instrument::Instrument(const char *name, Note defaultNote, std::array<uint8_t, 2> cc)
    : _defaultNote(defaultNote)
    , _notes()
    , _activeNotes()
    , _cc(cc)
{
    strcpy(_name, name);
    clear();
}

const char *Instrument::getName() const
{
    return _name;
}

const Note &Instrument::getDefaultNote() const
{
    return _defaultNote;
}

void Instrument::setDefaultNote(Note defaultNote)
{
    _defaultNote = defaultNote;
    for (uint8_t i = 0; i < s_stepNumber; ++i) {
        if (!isActiveNote(i)) {
            setNote(i, defaultNote);
        }
    }
}

bool Instrument::isActiveNote(uint8_t idx) const
{
    return _activeNotes[idx];
}

const Note& Instrument::getNote(uint8_t idx) const
{
    return _notes[idx];
}

void Instrument::setNote(uint8_t idx, Note note) 
{
    _notes[idx] = note;
}

void Instrument::toggleNote(uint8_t idx)
{
    _activeNotes[idx] = !_activeNotes[idx];
}

void Instrument::clear()
{
    for (uint8_t i = 0; i < s_stepNumber; ++i) {
        _notes[i] = _defaultNote;
        _activeNotes[i] = false;
    }
}

uint8_t Instrument::getCC(uint8_t idx) const
{
    return _cc[idx];
}