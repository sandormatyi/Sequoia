#pragma once

#include "LedBase.h"

class Adafruit_MCP23017;

class MCPLed : public LedBase
{
public:
    MCPLed(Adafruit_MCP23017 &mcp, uint8_t pin, bool inverted = false);
    virtual ~MCPLed() = default;
    void init() override;

protected:
    void _turnOn() override;
    void _turnOff() override;
    void _setPWMValue(uint16_t value) override;

private:
    Adafruit_MCP23017 &_mcp;
    uint8_t _pin;
    bool _inverted;
};