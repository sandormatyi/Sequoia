#pragma once

#include <inttypes.h>
#include "LedBase.h"

class LazyLedBase : public LedBase
{
public:
    virtual ~LazyLedBase() = default;

    void init() final { _init(); _turnOff(); }
    void turnOn() final;
    void turnOff() final;
    void setPWMValue(uint8_t percent) final;
    void update() final;

protected:
    LazyLedBase(uint8_t pin, uint8_t inverted);
    //LazyLedBase(const LazyLedBase &) = delete;
    //LazyLedBase(const LazyLedBase &&) = delete;

    enum class State
    {
        Init,
        On,
        Off,
        PWM
    };

    struct Fields {
        uint32_t pwmPercent : 7;
        uint32_t nextPwmPercent : 7;
        uint32_t currentState : 2;
        uint32_t nextState : 2;

        uint32_t pin : 10;
        uint32_t inverted : 1;
    } _fields;
};