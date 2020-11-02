#include "Hardware/Led/MCPLed.h"
#include "Adafruit_MCP23017.h"
#include "DBG.h"

MCPLed::MCPLed(Adafruit_MCP23017& mcp, uint8_t pin, bool inverted /*= false*/)
    : LazyLedBase(pin, inverted)
    , _mcp(mcp)
{
  // DBG("MCP Led on pin %d\n", _fields.pin);
}

void MCPLed::_init()
{
  _mcp.pinMode(_fields.pin, OUTPUT);
  _mcp.pullUp(_fields.pin, LOW);
}

void MCPLed::_turnOn()
{
  _mcp.digitalWrite(_fields.pin, _fields.inverted ? LOW : HIGH);
}

void MCPLed::_turnOff()
{
  _mcp.digitalWrite(_fields.pin, _fields.inverted ? HIGH : LOW);
}

void MCPLed::_setPWMValue(float normalizedValue)
{
}