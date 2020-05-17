
// Please read Bounce_MCP.h for information about the liscence and authors

#include <Arduino.h>
#include "Hardware/Button/Bounce_MCP.h"


Bounce_MCP::Bounce_MCP(Adafruit_MCP23017& mcp, uint8_t pin,unsigned long interval_millis) 
	: _mcp(mcp)
{
	interval(interval_millis);
	previous_millis = millis();
	state = HIGH;
    this->pin = pin;
}


void Bounce_MCP::write(int new_state)
{
	this->state = new_state;
	_mcp.digitalWrite(pin,state);
}


void Bounce_MCP::interval(unsigned long interval_millis)
{
  this->interval_millis = interval_millis;
  this->rebounce_millis = 0;
}

void Bounce_MCP::rebounce(unsigned long interval)
{
	 this->rebounce_millis = interval;
}

int Bounce_MCP::update()
{
	if ( debounce() ) {
        rebounce(0);
        return stateChanged = 1;
    }

     // We need to rebounce, so simulate a state change
     
	if ( rebounce_millis && (millis() - previous_millis >= rebounce_millis) ) {
        previous_millis = millis();
		 rebounce(0);
		 return stateChanged = 1;
	}

	return stateChanged = 0;
}


unsigned long Bounce_MCP::duration()
{
  return millis() - previous_millis;
}


int Bounce_MCP::read()
{
	return (int)state;
}


// Protected: debounces the pin
int Bounce_MCP::debounce() {
	
	uint8_t newState = _mcp.digitalRead(pin);
	if (state != newState ) {
  		if (millis() - previous_millis >= interval_millis) {
  			previous_millis = millis();
  			state = newState;
  			return 1;
	}
  }
  
  return 0;
	
}

// The risingEdge method is true for one scan after the de-bounced input goes from off-to-on.
bool  Bounce_MCP::risingEdge() { return stateChanged && state; }
// The fallingEdge  method it true for one scan after the de-bounced input goes from on-to-off. 
bool  Bounce_MCP::fallingEdge() { return stateChanged && !state; }

