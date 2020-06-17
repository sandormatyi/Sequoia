#pragma once

#include <inttypes.h>

class I2C_eeprom;

void dumpEEPROM(I2C_eeprom& ee, uint16_t memoryAddress, uint16_t length);
void testEeprom(I2C_eeprom& ee);
