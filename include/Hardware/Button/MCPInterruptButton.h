#pragma once

#include "ButtonBase.h"
#include <inttypes.h>

class Adafruit_MCP23017;

class MCPInterruptButton : public ButtonBase
{
public:
    MCPInterruptButton(Adafruit_MCP23017 &mcp, uint8_t pin);
    virtual ~MCPInterruptButton() = default;

    void init() override;
    int update() override;
    int read() override;
    bool risingEdge() override;
    bool fallingEdge() override;

private:
    Adafruit_MCP23017 &_mcp;
    uint8_t _pin;
    uint8_t _state;
};