#include "Hardware/Led/LazyLedBase.h"

 LazyLedBase::LazyLedBase(uint8_t pin, uint8_t inverted)
    : _fields{0.f, 0.f, 0, 0, pin, inverted}
{
}


void LazyLedBase::turnOn()
{
    _fields.nextState = (uint32_t)State::On;
}

void LazyLedBase::turnOff()
{
    _fields.nextState = (uint32_t)State::Off;
}

void LazyLedBase::setPWMValue(float normalizedValue)
{
    _fields.nextState = (uint32_t)State::PWM;
    _fields.nextPwmValue = normalizedValue;
}

void LazyLedBase::update()
{
    if (_fields.nextState != _fields.currentState || (_fields.nextState == (uint32_t)State::PWM && _fields.nextPwmValue != _fields.pwmValue))
    {
        switch (State(_fields.nextState))
        {
        case State::On:
            _turnOn();
            break;
        case State::Off:
            _turnOff();
            break;
        case State::PWM:
            _setPWMValue(_fields.nextPwmValue);
            _fields.pwmValue = _fields.nextPwmValue;
            break;
        default:
            break;
        }
        _fields.currentState = _fields.nextState;
    }
}