#pragma once

#include "Sequencer.h"
#include <array>
#include <inttypes.h>

struct CC {
  static constexpr uint8_t s_globalCCChannel = 5;

  static constexpr std::array<uint8_t, 2> globalControls{20, 21};
  static constexpr std::array<std::array<uint8_t, 2>, Sequencer::s_instrumentNumber> instrumentControls{
      std::array<uint8_t, 2>{22, 23},
      std::array<uint8_t, 2>{24, 25},
      std::array<uint8_t, 2>{26, 27},
      std::array<uint8_t, 2>{28, 29}};
};

class Peripherals;

void sendAllCCValues(Peripherals& p, Sequencer& seq);