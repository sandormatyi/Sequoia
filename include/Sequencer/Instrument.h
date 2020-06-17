#pragma once

#include <inttypes.h>
#include <array>
#include "Sequencer/Note.h"

enum Scale { Chromatic, Major, Minor, Harmonic_minor, Unison };
const char scaleNames[5][4] = {"Chr", "Maj", "Min", "HMn", "Uni"};

class Instrument
{
public:
    static constexpr uint8_t s_stepNumber = 16;
    static constexpr uint16_t s_patternSizeInBytes = s_stepNumber * sizeof(Note);

    Instrument(uint8_t channel, const char *name, Note defaultNote);

    const char* getName() const;
    const Note &getDefaultNote() const;
    void setDefaultNote(Note defaultNote);
    uint8_t getChannel() const;

    bool isActiveNote(uint8_t idx) const;

    const Note& getNote(uint8_t idx) const;
    void setNote(uint8_t idx, Note note);
    void toggleNote(uint8_t idx);

    void clear();

    void randomize(Scale scale, float probability);

    void importPattern(const std::array<byte, s_patternSizeInBytes>& pattern);
    std::array<byte, s_patternSizeInBytes> exportPattern() const;

private:
    uint8_t _channel;
    char _name[7];
    Note _defaultNote;
    std::array<Note, s_stepNumber> _notes;
    std::array<bool, s_stepNumber> _activeNotes;
};