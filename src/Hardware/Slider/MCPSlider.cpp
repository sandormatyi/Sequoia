#include "Hardware/Slider/MCPSlider.h"
#include <Arduino.h>

MCPSlider::MCPSlider(Adafruit_MCP3008& adc, uint8_t channel)
    : _adc(adc)
    , _channel(channel)
{
}

bool MCPSlider::update()
{    
    auto newValue = _adc.readADC(_channel);
    newValue = _adc.readADC(_channel);
    newValue = constrain(newValue, s_deadband, s_maxValue - s_deadband);
    newValue = map(newValue, s_deadband, s_maxValue - s_deadband, 0, s_maxValue);
    if ((newValue < (_value - s_tolerance)) || (newValue > (_value + s_tolerance))) {
        _value = newValue;
        return true;
    }
    return false;
}

float MCPSlider::readNormalizedRawValue() const
{
    return getNormalizedValue(_value);
}

float MCPSlider::getNormalizedValue(int value) const
{
    return float(value) / s_maxValue;
}