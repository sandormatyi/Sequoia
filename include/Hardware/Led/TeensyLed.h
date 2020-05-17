#pragma once

#include "LedBase.h"

class TeensyLed : public LedBase
{
public:
    TeensyLed(uint8_t pin, bool inverted = false);
    virtual ~TeensyLed() = default;
    void init() override;

protected:
    void _turnOn() override;
    void _turnOff() override;
    void _setPWMValue(uint16_t value) override;

private:
    uint8_t _pin;
    bool _inverted;
};