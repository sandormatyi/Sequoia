#include "Sequencer/PresetHandler.h"
#include <I2C_eeprom.h>
#include "DBG.h"

void PresetHandler::printBlock(const std::array<uint8_t, PresetHandler::s_presetSize>& data) 
{
    for(int i = 0; i < s_presetSize; ++i) {
        DBG("%d ", data[i]);
    }
    DBG("\n");
}

void PresetHandler::savePresetToBank(I2C_eeprom& ee, uint8_t bankIdx, uint8_t presetIdx, const std::array<uint8_t, s_presetSize>& data)
{
    const auto address = s_firstPresetAddress + ((bankIdx * s_presetPerBank) + presetIdx) * s_presetSize;
    DBG("EEPROM - Writing to address %d: ", address);
    printBlock(data);
    ee.writeBlock(address, data.data(), data.size());
}

std::array<uint8_t, PresetHandler::s_presetSize> PresetHandler::loadPresetFromBank(I2C_eeprom& ee, uint8_t bankIdx, uint8_t presetIdx)
{
    const auto address = s_firstPresetAddress + ((bankIdx * s_presetPerBank) + presetIdx) * s_presetSize;
    DBG("EEPROM - Reading from address %d: ", address);
    std::array<uint8_t, PresetHandler::s_presetSize> data;
    ee.readBlock(address, data.data(), PresetHandler::s_presetSize);
    printBlock(data);
    return data;
}