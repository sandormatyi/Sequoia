#include "Hardware/Button/TeensyButton.h"
#include <Arduino.h>

TeensyButton::TeensyButton(uint8_t pin, unsigned long interval_millis) 
    : _pin(pin)
    , _bounce(pin, interval_millis)
{
}

void TeensyButton::init() 
{ 
    pinMode(_pin, INPUT_PULLUP); 
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