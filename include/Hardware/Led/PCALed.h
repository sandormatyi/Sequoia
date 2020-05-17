#pragma once

#include "LedBase.h"

class PCA9685;

class PCALed : public LedBase
{
public:
    PCALed(PCA9685 &pca, uint8_t pin, bool inverted = false);
    virtual ~PCALed() = default;

protected:
    void _turnOn() override;
    void _turnOff() override;
    void _setPWMValue(uint16_t value) override;

private:
    PCA9685 &_pca;
    uint8_t _pin;
    bool _inverted;
};