#pragma once

#include <inttypes.h>

class LedBase
{
public:
    virtual ~LedBase() = default;

    virtual void init() {}
    void turnOn();
    void turnOff();
    void setPWMValue(uint16_t value);
    void update();

protected:
    LedBase(bool lazy) : _lazy(lazy) {}
    LedBase(const LedBase &) = delete;
    LedBase(const LedBase &&) = delete;

    virtual void _turnOn() = 0;
    virtual void _turnOff() = 0;
    virtual void _setPWMValue(uint16_t value) = 0;

private:
    const bool _lazy;

    enum class State
    {
        Init,
        On,
        Off,
        PWM
    };
    int _pwmValue;
    State _currentState = State::Init;
    State _nextState = State::Init;
};