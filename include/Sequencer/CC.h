#pragma once

#include <inttypes.h>
#include <array>
#include "Sequencer.h"

struct CC {
    static constexpr uint8_t s_globalCCChannel = 5;
    
    static constexpr std::array<uint8_t, 4> globalControls {20, 23, 26, 29};
    static constexpr std::array<std::array<uint8_t, 2>, Sequencer::s_instrumentNumber> instrumentControls {
        std::array<uint8_t, 2>{21, 22},
        std::array<uint8_t, 2>{24, 25},
        std::array<uint8_t, 2>{27, 28},
        std::array<uint8_t, 2>{30, 31}
    };
};