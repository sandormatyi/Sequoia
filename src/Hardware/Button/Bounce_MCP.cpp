
// Please read Bounce_MCP.h for information about the liscence and authors

#include <Arduino.h>
#include "Hardware/Button/Bounce_MCP.h"


Bounce_MCP::Bounce_MCP(Adafruit_MCP23017& mcp, uint8_t pin, uint8_t interval_millis) 
	: _mcp(mcp)
	, previous_millis(millis())
	, _interval_millis(interval_millis)
	, _pin(pin)
	, _state(HIGH)
	, _stateChanged(0)
{
}

void Bounce_MCP::write(int new_state)
{
	_state = new_state;
	_mcp.digitalWrite(_pin, _state);
}

void Bounce_MCP::interval(uint8_t interval_millis)
{
  _interval_millis = interval_millis;
}

int Bounce_MCP::update()
{
	_stateChanged = debounce();
	return _stateChanged;
}

unsigned long Bounce_MCP::duration()
{
  return millis() - previous_millis;
}

int Bounce_MCP::read()
{
	return (int)_state;
}

// Protected: debounces the pin
int Bounce_MCP::debounce() {
	
	uint8_t newState = _mcp.digitalRead(_pin);
	if (_state != newState ) {
  		if (millis() - previous_millis >= _interval_millis) {
  			previous_millis = millis();
  			_state = newState;
  			return 1;
	}
  }
  
  return 0;
}

// The risingEdge method is true for one scan after the de-bounced input goes from off-to-on.
bool  Bounce_MCP::risingEdge() { return _stateChanged && _state; }
// The fallingEdge  method it true for one scan after the de-bounced input goes from on-to-off. 
bool  Bounce_MCP::fallingEdge() { return _stateChanged && !_state; }

Adafruit_MCP23017 &Bounce_MCP::getMCP() { return _mcp; }

uint8_t Bounce_MCP::getPin() { return _pin; }