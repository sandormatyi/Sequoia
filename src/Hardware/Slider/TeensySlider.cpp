#include "Hardware/Slider/TeensySlider.h"
#include <Arduino.h>

TeensySlider::TeensySlider(uint8_t pin)
    : _pin(pin)
{
}

void TeensySlider::init()
{
    pinMode(_pin, INPUT);
}

bool TeensySlider::update()
{    
    auto newValue = analogRead(_pin);
    newValue = analogRead(_pin);
    newValue = constrain(newValue, s_deadband, s_maxValue - s_deadband);
    newValue = map(newValue, s_deadband, s_maxValue - s_deadband, 0, s_maxValue);
    if ((newValue < (_value - s_tolerance)) || (newValue > (_value + s_tolerance))) {
        _value = newValue;
        return true;
    }
    return false;
}

float TeensySlider::readNormalizedRawValue() const
{
    return getNormalizedValue(_value);
}

float TeensySlider::getNormalizedValue(int value) const
{
    return float(value) / s_maxValue;
}