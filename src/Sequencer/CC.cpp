#include "Sequencer/CC.h"

constexpr std::array<uint8_t, 2> CC::globalControls;
constexpr std::array<std::array<uint8_t, 2>, Sequencer::s_instrumentNumber> CC::instrumentControls;