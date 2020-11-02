#pragma once

#include "LedBase.h"

class TeensyLed : public LedBase
{
public:
  TeensyLed(uint8_t pin, bool inverted = false);
  virtual ~TeensyLed() = default;

protected:
  void _init() override;
  void _turnOn() override;
  void _turnOff() override;
  void _setPWMValue(float normalizedValue) override;

private:
  struct Fields {
    uint8_t pin : 5;
    uint8_t inverted : 1;
  } _fields;
};