#include "Sequencer/Instrument.h"
#include <cstring>
#include <limits>
#include <Arduino.h>

static float getRandomFloat() 
{
    return (float(random()) / float(RAND_MAX));
}

namespace Scales {
static constexpr std::array<uint8_t, 12> chromatic {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static constexpr std::array<uint8_t, 7> major {0, 2, 4, 5, 7, 9, 11};
static constexpr std::array<uint8_t, 7> minor {0, 2, 3, 5, 7, 8, 10};
static constexpr std::array<uint8_t, 7> harmonicMinor {0, 2, 3, 5, 7, 8, 11};

uint8_t getRelativeNote(Scale scale, float random) 
{
    switch(scale) {
    case Chromatic:
        return chromatic[int(random * chromatic.size())];
    case Major:
        return major[int(random * major.size())];
    case Minor:
        return minor[int(random * minor.size())];
    case Harmonic_minor:
        return harmonicMinor[int(random * harmonicMinor.size())];
    default:
        return 0;
    }
}
}

Instrument::Instrument(uint8_t channel, const char *name, Note defaultNote)
    : _channel(channel)
    , _defaultNote(defaultNote)
    , _notes()
    , _activeNotes()
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

uint8_t Instrument::getChannel() const
{
    return _channel;
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

void Instrument::randomize(Scale scale, float probability) 
{
    clear();
    for (uint8_t i = 0; i < s_stepNumber; ++i) {
        if (getRandomFloat() > probability) 
            continue;

        const auto randomOctave = getRandomFloat();
        const auto octaveOffset = scale == Unison ? 0
                                : randomOctave < 0.3f ? -1
                                : randomOctave > 0.7f ? 1
                                : 0;

        const auto noteOffset = Scales::getRelativeNote(scale, getRandomFloat());
        _notes[i] = _defaultNote;
        _notes[i]._noteNumber += (noteOffset + 12 * octaveOffset);
        _activeNotes[i] = true;
    }
}

void Instrument::importPattern(const std::array<byte, Instrument::s_patternSizeInBytes>& pattern)
{
    clear();
    for(int i = 0; i < s_stepNumber; ++i) {
        if (pattern[i * 2 + 1] > 0) {
            _activeNotes[i] = true;
            _notes[i]._velocity = _defaultNote._velocity;
        } else {
            _notes[i]._velocity = pattern[i * 2 + 1];
        }
        _notes[i]._noteNumber = pattern[i * 2];
    }
}

std::array<byte, Instrument::s_patternSizeInBytes> Instrument::exportPattern() const
{
    std::array<byte, Instrument::s_patternSizeInBytes> pattern;
    for(int i = 0; i < s_stepNumber; ++i) {
        pattern[i * 2] = _notes[i]._noteNumber;
        pattern[i * 2 + 1] = _activeNotes[i] ? _notes[i]._velocity : 0;
    }
    return pattern;
}