#pragma once

#include "LazyLedBase.h"

class Adafruit_MCP23017;

class MCPLed : public LazyLedBase
{
public:
  MCPLed(Adafruit_MCP23017& mcp, uint8_t pin, bool inverted = false);
  virtual ~MCPLed() = default;

protected:
  void _init() override;
  void _turnOn() override;
  void _turnOff() override;
  void _setPWMValue(float normalizedValue) override;

private:
  Adafruit_MCP23017& _mcp;
};