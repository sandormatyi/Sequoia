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
    _setPWMValue(PCA9685_FULL_ON); 
}

void PCALed::_turnOff()
{ 
    _setPWMValue(PCA9685_FULL_OFF); 
}

void PCALed::_setPWMValue(uint16_t value)
{ 
    _pca.getPin(_pin).setValueAndWrite(_inverted ? value : PCA9685_MAX_VALUE - value); 
}