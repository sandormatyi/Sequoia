#pragma once

#include "LazyLedBase.h"

class PCA9685;

class PCALed : public LazyLedBase
{
public:
    PCALed(PCA9685 &pca, uint8_t pin, bool inverted = false);
    virtual ~PCALed() = default;

protected:
    void _turnOn() override;
    void _turnOff() override;
    void _setPWMValue(float normalizedValue) override;

private:
    PCA9685 &_pca;
};