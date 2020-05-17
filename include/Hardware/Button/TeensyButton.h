#pragma once

#include "ButtonBase.h"
#include "Bounce.h"

class TeensyButton : public ButtonBase
{
public:
    TeensyButton(uint8_t pin, unsigned long interval_millis);
    virtual ~TeensyButton() = default;

    void init() override;
    int update() override;
    int read() override;
    bool risingEdge() override;
    bool fallingEdge() override;

private:
    uint8_t _pin;
    Bounce _bounce;
};

