#pragma once

#include "Sequencer/Instrument.h"
#include <array>
#include <inttypes.h>
#include <vector>

class Sequencer
{
public:
  static constexpr uint8_t s_instrumentNumber = 4;

  Sequencer();
  ~Sequencer();

  Instrument& getInstrument(uint8_t idx);
  void clearInstruments();

  uint8_t getCurrentInstrumentIdx() const;
  void setCurrentInstrument(uint8_t idx);
  Instrument& getCurrentInstrument();

  std::vector<Note> getNotes(uint8_t stepIdx);

  bool isInstrumentMuted(uint8_t idx) const;
  void muteInstrument(uint8_t idx, bool isMuted);
  void muteAllInstruments(bool isMuted);

private:
  std::array<Instrument, s_instrumentNumber> _instruments;
  std::array<bool, s_instrumentNumber> _isMuted;
  uint8_t _currentInstrument = 0;
};