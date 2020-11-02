#pragma once

#include "Bounce_MCP.h"
#include "ButtonBase.h"
#include <inttypes.h>

class Adafruit_MCP23017;

class MCPButton : public ButtonBase
{
public:
  MCPButton(Adafruit_MCP23017& mcp, uint8_t pin, uint8_t interval_millis);
  virtual ~MCPButton() = default;

  void init() override;
  int update() override;
  int read() override;
  bool risingEdge() override;
  bool fallingEdge() override;

private:
  Bounce_MCP _bounce;
};