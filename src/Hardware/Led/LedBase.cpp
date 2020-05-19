#include "Hardware/Led/LedBase.h"

void LedBase::turnOn()
{
    _nextState = State::On;
    if (!_lazy)
        update();
}

void LedBase::turnOff()
{
    _nextState = State::Off;
    if (!_lazy)
        update();
}

void LedBase::setPWMValue(uint8_t percent)
{
    _nextState = State::PWM;
    _pwmPercent = percent;
    if (!_lazy)
        update();
}

void LedBase::update()
{
    if (_nextState != _currentState)
    {
        switch (_nextState)
        {
        case State::On:
            _turnOn();
            break;
        case State::Off:
            _turnOff();
            break;
        case State::PWM:
            _setPWMValue(_pwmPercent);
            break;
        default:
            break;
        }
        _currentState = _nextState;
    }
}