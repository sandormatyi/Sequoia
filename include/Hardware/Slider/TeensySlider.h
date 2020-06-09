#pragma once

#include "SliderBase.h"

class TeensySlider : public SliderBase
{
public:
    TeensySlider(uint8_t pin);
    virtual ~TeensySlider() = default;

    void init() override;
    bool update() override;

    float readNormalizedRawValue() const override;

private:
    float getNormalizedValue(int value) const;

    static constexpr int s_maxValue = 1023;
    static constexpr int s_deadband = 24;
    static constexpr int s_tolerance = 6;
    uint8_t _pin;
    uint16_t _value;
};