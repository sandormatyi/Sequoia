#pragma once

#include <inttypes.h>
#include <array>
#include <vector>
#include "Sequencer/Instrument.h"

class Sequencer 
{
public:
    static constexpr uint8_t s_instrumentNumber = 8;

    Sequencer();
    ~Sequencer();

    Instrument& getInstrument(uint8_t idx);
    void clearInstrument(uint8_t idx);
    void clearInstruments();

    std::vector<Note> getNotes(uint8_t beatNumber);

    void muteInstrument(uint8_t idx, bool isMuted);
    void muteAllInstruments(bool isMuted);

private:
    std::array<Instrument*, s_instrumentNumber> _instruments;
    std::array<bool, s_instrumentNumber> _isMuted;
};