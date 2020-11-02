#include "Hardware/Button/TeensyButton.h"
#include <Arduino.h>

TeensyButton::TeensyButton(uint8_t pin, uint8_t interval_millis)
    : _bounce(pin, interval_millis)
{
}

void TeensyButton::init()
{
  pinMode(_bounce.getPin(), INPUT_PULLUP);
}

int TeensyButton::update()
{
  return _bounce.update();
}

int TeensyButton::read()
{
  return _bounce.read();
}

bool TeensyButton::risingEdge()
{
  return _bounce.risingEdge();
}

bool TeensyButton::fallingEdge()
{
  return _bounce.fallingEdge();
}