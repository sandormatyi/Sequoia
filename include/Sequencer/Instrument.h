#pragma once

#include <inttypes.h>
#include <array>
#include "Sequencer/Note.h"

class Instrument
{
public:
    static constexpr uint8_t s_beatNumber = 16;

    Instrument(const char *name, Note defaultNote, std::array<uint8_t, 2> cc);

    const char* getName() const;
    const Note &getDefaultNote() const;
    void setDefaultNote(Note defaultNote);

    bool isActiveNote(uint8_t idx) const;

    const Note& getNote(uint8_t idx) const;
    void setNote(uint8_t idx, Note note);
    void toggleNote(uint8_t idx);

    uint8_t getCC(uint8_t idx) const;

    void clear();

private:
    char _name[5];
    Note _defaultNote;
    std::array<uint8_t, 2> _cc;
    std::array<Note, s_beatNumber> _notes;
    std::array<bool, s_beatNumber> _activeNotes;
};