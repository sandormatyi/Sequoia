#include "Hardware/Led/PCALed.h"
#include "DBG.h"
#include "Hardware/Led/_LUT.h"
#include <PCA9685.h>

static uint16_t gammaCorrection(uint16_t value, uint16_t maxValue)
{
  return _lut[value];
}

PCALed::PCALed(PCA9685& pca, uint8_t pin, bool inverted /*= false*/)
    : LazyLedBase(pin, inverted)
    , _pca(pca)
{
  // DBG("PCA Led on pin %d\n", _fields.pin);
}

void PCALed::_turnOn()
{
  if (_fields.inverted) {
    _pca.getPin(_fields.pin).fullOnAndWrite();
  } else {
    _pca.getPin(_fields.pin).fullOffAndWrite();
  }
}

void PCALed::_turnOff()
{
  if (_fields.inverted) {
    _pca.getPin(_fields.pin).fullOffAndWrite();
  } else {
    _pca.getPin(_fields.pin).fullOnAndWrite();
  }
}

void PCALed::_setPWMValue(float normalizedValue)
{
  uint16_t value = round(normalizedValue * PCA9685_MAX_VALUE);
  uint16_t gammaCorrectedValue = gammaCorrection(value, PCA9685_MAX_VALUE);
  DBG("Pin %d PWM value set to %d (%d%%)\n", _fields.pin, gammaCorrectedValue, int(normalizedValue * 100));
  _pca.getPin(_fields.pin).setValueAndWrite(_fields.inverted ? gammaCorrectedValue : PCA9685_MAX_VALUE - gammaCorrectedValue);
}