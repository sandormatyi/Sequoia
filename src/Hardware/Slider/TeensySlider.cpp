#include "Hardware/Slider/TeensySlider.h"
#include <Arduino.h>

TeensySlider::TeensySlider(uint8_t pin, uint8_t nLevels)
    : _pin(pin)
    , _nLevels(nLevels)
{
}

void TeensySlider::init()
{
    pinMode(_pin, INPUT);
}

bool TeensySlider::update()
{
    const auto newValue = analogRead(_pin);
    const auto newLevel = getLevel(newValue);
    const auto currentLevel = getLevel(_value);
    _value = newValue;

    return currentLevel != newLevel;
}

uint16_t TeensySlider::readLevel() const
{
    return getLevel(_value);
}

float TeensySlider::readNormalizedRawValue() const
{
    return getNormalizedValue(_value);
}

uint16_t TeensySlider::getLevel(int value) const
{
    return round(float(value * _nLevels) / s_maxValue);
}

float TeensySlider::getNormalizedValue(int value) const
{
    return float(value) / s_maxValue;
}