#include "Hardware/Led/LazyLedBase.h"

 LazyLedBase::LazyLedBase(uint8_t pin, uint8_t inverted)
    : _fields{0, 0, 0, 0, pin, inverted}
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

void LazyLedBase::setPWMValue(uint8_t percent)
{
    _fields.nextState = (uint32_t)State::PWM;
    _fields.nextPwmPercent = percent;
}

void LazyLedBase::update()
{
    if (_fields.nextState != _fields.currentState || (_fields.nextState == (uint32_t)State::PWM && _fields.nextPwmPercent != _fields.pwmPercent))
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
            _setPWMValue(_fields.nextPwmPercent);
            _fields.pwmPercent = _fields.nextPwmPercent;
            break;
        default:
            break;
        }
        _fields.currentState = _fields.nextState;
    }
}