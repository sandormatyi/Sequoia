#pragma once

#include <inttypes.h>

class LedBase
{
public:
    virtual ~LedBase() = default;

    void init() { _init(); _turnOff(); }
    void turnOn();
    void turnOff();
    void setPWMValue(uint8_t percent);
    void update();

protected:
    LedBase(bool lazy) : _lazy(lazy) {}
    LedBase(const LedBase &) = delete;
    LedBase(const LedBase &&) = delete;

    virtual void _init() {}
    virtual void _turnOn() = 0;
    virtual void _turnOff() = 0;
    virtual void _setPWMValue(uint8_t percent) = 0;

private:
    const bool _lazy;

    enum class State : uint8_t
    {
        Init,
        On,
        Off,
        PWM
    };
    uint8_t _pwmPercent;
    State _currentState = State::Init;
    State _nextState = State::Init;
};