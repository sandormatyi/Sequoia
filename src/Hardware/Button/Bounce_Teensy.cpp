
#include "Hardware/Button/Bounce_Teensy.h"
#include <Arduino.h>

Bounce_Teensy::Bounce_Teensy(uint8_t pin, uint8_t interval_millis)
{
  interval(interval_millis);
  previous_millis = millis();
  state = digitalRead(pin);
  this->pin = pin;
}

void Bounce_Teensy::write(int new_state)
{
  this->state = new_state;
  digitalWrite(pin, state);
}

void Bounce_Teensy::interval(uint8_t interval_millis)
{
  this->interval_millis = interval_millis;
}

int Bounce_Teensy::update()
{
  if (debounce()) {
    return stateChanged = 1;
  }
  return stateChanged = 0;
}

unsigned long Bounce_Teensy::duration()
{
  return millis() - previous_millis;
}

int Bounce_Teensy::read()
{
  return (int)state;
}

// Protected: debounces the pin
int Bounce_Teensy::debounce()
{

  uint8_t newState = digitalRead(pin);
  if (state != newState) {
    if (millis() - previous_millis >= interval_millis) {
      previous_millis = millis();
      state = newState;
      return 1;
    }
  }

  return 0;
}

// The risingEdge method is true for one scan after the de-bounced input goes from off-to-on.
bool Bounce_Teensy::risingEdge() { return stateChanged && state; }
// The fallingEdge  method it true for one scan after the de-bounced input goes from on-to-off.
bool Bounce_Teensy::fallingEdge() { return stateChanged && !state; }
