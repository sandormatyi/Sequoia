#include "Hardware/Button/MCPButton.h"
#include "Adafruit_MCP23017.h"

MCPButton::MCPButton(Adafruit_MCP23017& mcp, uint8_t pin, uint8_t interval_millis)
    : _bounce(mcp, pin, interval_millis)
{
}

void MCPButton::init()
{
  _bounce.getMCP().pinMode(_bounce.getPin(), INPUT);
  _bounce.getMCP().pullUp(_bounce.getPin(), HIGH);
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