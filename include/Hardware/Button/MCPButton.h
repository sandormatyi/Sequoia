#pragma once

#include "ButtonBase.h"
#include <inttypes.h>
#include "Bounce_MCP.h"

class Adafruit_MCP23017;

class MCPButton : public ButtonBase
{
public:
    MCPButton(Adafruit_MCP23017 &mcp, uint8_t pin, unsigned long interval_millis);
    virtual ~MCPButton() = default;

    void init() override;
    int update() override;
    int read() override;
    bool risingEdge() override;
    bool fallingEdge() override;

private:
    Adafruit_MCP23017 &_mcp;
    uint8_t _pin;
    Bounce_MCP _bounce;
};