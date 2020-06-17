#include "Sequencer/PresetHandler.h"
#include <I2C_eeprom.h>

void PresetHandler::savePresetToBank(I2C_eeprom& ee, uint8_t bankIdx, uint8_t presetIdx, const std::array<uint8_t, s_presetSize>& data)
{
    const auto address = s_firstPresetAddress + ((bankIdx * s_presetPerBank) + presetIdx) * s_presetSize;
    ee.writeBlock(address, data.data(), data.size());
}

std::array<uint8_t, PresetHandler::s_presetSize> PresetHandler::loadPresetFromBank(I2C_eeprom& ee, uint8_t bankIdx, uint8_t presetIdx)
{
    const auto address = s_firstPresetAddress + ((bankIdx * s_presetPerBank) + presetIdx) * s_presetSize;
    std::array<uint8_t, PresetHandler::s_presetSize> data;
    ee.readBlock(address, data.data(), data.size());
    return data;
}