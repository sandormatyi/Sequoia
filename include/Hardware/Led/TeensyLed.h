#pragma once

#include "LedBase.h"

class TeensyLed : public LedBase
{
public:
    TeensyLed(uint8_t pin, bool inverted = false);
    virtual ~TeensyLed() = default;

protected:
    void _init() override;
    void _turnOn() override;
    void _turnOff() override;
    void _setPWMValue(uint8_t percent) override;

private:
    uint8_t _pin;
    bool _inverted;
};