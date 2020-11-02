#include "Hardware/Led/TeensyLed.h"
#include "Arduino.h"

TeensyLed::TeensyLed(uint8_t pin, bool inverted /*= false*/)
    : LedBase()
    , _fields{pin, inverted}
{
}

void TeensyLed::_init()
{
  pinMode(_fields.pin, OUTPUT);
}

void TeensyLed::_turnOn()
{
  digitalWrite(_fields.pin, _fields.inverted ? LOW : HIGH);
}

void TeensyLed::_turnOff()
{
  digitalWrite(_fields.pin, _fields.inverted ? HIGH : LOW);
}

void TeensyLed::_setPWMValue(float normalizedValue)
{
  int value = round(normalizedValue * 255);
  switch (_fields.pin) {
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
    analogWrite(_fields.pin, _fields.inverted ? 256 - value : value);
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