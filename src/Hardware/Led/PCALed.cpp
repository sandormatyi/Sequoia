#include "Hardware/Led/PCALed.h"
#include <PCA9685.h>

PCALed::PCALed(PCA9685 &pca, uint8_t pin, bool inverted /*= false*/) 
    : LedBase(true)
    , _pca(pca)
    , _pin(pin)
    , _inverted(inverted) 
{
}

void PCALed::_turnOn() 
{
    if (_inverted) {
        _pca.getPin(_pin).fullOnAndWrite();
    } else {
        _pca.getPin(_pin).fullOffAndWrite();
    }
}

void PCALed::_turnOff()
{ 
    if (_inverted) {
        _pca.getPin(_pin).fullOffAndWrite();
    } else {
        _pca.getPin(_pin).fullOnAndWrite();
    }
}

void PCALed::_setPWMValue(uint8_t percent)
{
    uint16_t value = (uint16_t(percent) * PCA9685_MAX_VALUE) / 100;
    _pca.getPin(_pin).setValueAndWrite(_inverted ? value : PCA9685_MAX_VALUE - value); 
}