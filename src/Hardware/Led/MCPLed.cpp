#include "Hardware/Led/MCPLed.h"
#include "Adafruit_MCP23017.h"

MCPLed::MCPLed(Adafruit_MCP23017 &mcp, uint8_t pin, bool inverted /*= false*/)
    : LedBase(true)
    , _mcp(mcp)
    , _pin(pin)
    , _inverted(inverted)
{
}

void MCPLed::_init()
{
    _mcp.pinMode(_pin, OUTPUT);
    _mcp.pullUp(_pin, LOW);
}

void MCPLed::_turnOn()
{
    _mcp.digitalWrite(_pin, _inverted ? LOW : HIGH);
}

void MCPLed::_turnOff()
{
    _mcp.digitalWrite(_pin, _inverted ? HIGH : LOW);
}

void MCPLed::_setPWMValue(uint8_t percent)
{
}