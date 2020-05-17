#pragma once

#include <inttypes.h>
#include <array>
#include "Sequencer/Note.h"

class Instrument
{
public:
    static constexpr uint8_t s_beatNumber = 16;

    Instrument(const char *name, Note note);

    const char* getName() const;
    const Note& getNote() const;
    bool isBeatSet(uint8_t idx) const;
    void setBeat(uint8_t idx, bool value);
    void toggleBeat(uint8_t idx);
    void clear();

private:
    char _name[5];
    Note _note;
    std::array<bool, s_beatNumber> _beats;
};