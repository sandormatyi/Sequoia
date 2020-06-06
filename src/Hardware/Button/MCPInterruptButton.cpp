#include "Hardware/Button/MCPInterruptButton.h"
#include "Adafruit_MCP23017.h"

MCPInterruptButton::MCPInterruptButton(Adafruit_MCP23017 &mcp, uint8_t pin)
    : _mcp(mcp)
    , _pin(pin)
{
}

void MCPInterruptButton::init()
{
    _mcp.pinMode(_pin, INPUT);
    _mcp.pullUp(_pin, HIGH);  // turn on a 100K pullup internally
    _mcp.setupInterruptPin(_pin, FALLING); 
}

int MCPInterruptButton::update()
{
    return 0;
}

int MCPInterruptButton::read()
{
    return 0;
}

bool MCPInterruptButton::risingEdge() 
{
    return false;
}

bool MCPInterruptButton::fallingEdge() 
{
    return false;
}