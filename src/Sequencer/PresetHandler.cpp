#include "Sequencer/PresetHandler.h"
#include "DBG.h"
#include <I2C_eeprom.h>

void PresetHandler::printBlock(const std::array<uint8_t, PresetHandler::s_presetSize>& data)
{
  for (int i = 0; i < s_presetSize; ++i) {
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

std::array<bool, PresetHandler::s_presetPerBank> PresetHandler::getPresetStates(I2C_eeprom& ee, uint8_t bankIdx)
{
  std::array<bool, PresetHandler::s_presetPerBank> result;
  for (int i = 0; i < s_presetPerBank; i++) {
    result[i] = false;
    const auto& preset = loadPresetFromBank(ee, bankIdx, i);
    for (size_t j = 0; j < preset.size(); j++) {
      if (j % 2 == 1 && preset[j] > 0) {
        result[i] = true;
        break;
      }
    }
  }
  return result;
}
