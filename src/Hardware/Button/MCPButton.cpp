#include "Hardware/Button/MCPButton.h"
#include "Adafruit_MCP23017.h"

MCPButton::MCPButton(Adafruit_MCP23017 &mcp, uint8_t pin, unsigned long interval_millis) 
    : _mcp(mcp)
    , _pin(pin)
    , _bounce(mcp, pin, interval_millis) 
{
}

void MCPButton::init()
{
    _mcp.pinMode(_pin, INPUT);
    _mcp.pullUp(_pin, HIGH);
}

int MCPButton::update()
{ 
    return _bounce.update(); 
}

int MCPButton::read()
{ 
    return _bounce.read(); 
}

bool MCPButton::risingEdge() 
{ 
    return _bounce.risingEdge(); 
}

bool MCPButton::fallingEdge() 
{ 
    return _bounce.fallingEdge(); 
}