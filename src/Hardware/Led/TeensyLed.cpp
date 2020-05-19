#include "Hardware/Led/TeensyLed.h"
#include "Arduino.h"

TeensyLed::TeensyLed(uint8_t pin, bool inverted /*= false*/)
    : LedBase(false)
    , _pin(pin)
    , _inverted(inverted)
{
}

void TeensyLed::_init()
{
    pinMode(_pin, OUTPUT);
}

void TeensyLed::_turnOn()
{
    digitalWrite(_pin, _inverted ? LOW : HIGH);
}

void TeensyLed::_turnOff()
{
    digitalWrite(_pin, _inverted ? HIGH : LOW);
}

void TeensyLed::_setPWMValue(uint8_t percent)
{
    int value = percent * 256 / 100;
    switch (_pin)
    {
    case 3:
    case 4:
    case 6:
    case 9:
    case 10:
    case 16:
    case 17:
    case 20:
    case 22:
    case 23:
        analogWrite(_pin, _inverted ? 256 - value : value);
        break;
    default:
        // if (value < 128) {
        //     digitalWrite(_pin, LOW)
        // } else {
        //     digitalWrite(_pin, HIGH);
        // }
        break;
    }
}