#pragma once

#include "SliderBase.h"

class TeensySlider : public SliderBase
{
public:
    TeensySlider(uint8_t pin, uint8_t nLevels);
    virtual ~TeensySlider() = default;

    void init() override;
    bool update() override;

    uint16_t readLevel() const override;
    float readNormalizedRawValue() const override;

private:
    uint16_t getLevel(int value) const;
    float getNormalizedValue(int value) const;

    static constexpr int s_maxValue = 1023;
    uint8_t _pin;
    uint8_t _nLevels;
    uint16_t _value;
};