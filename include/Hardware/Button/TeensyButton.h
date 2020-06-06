#pragma once

#include "ButtonBase.h"
#include "Bounce_Teensy.h"

class TeensyButton : public ButtonBase
{
public:
    TeensyButton(uint8_t pin, uint8_t interval_millis);
    virtual ~TeensyButton() = default;

    void init() override;
    int update() override;
    int read() override;
    bool risingEdge() override;
    bool fallingEdge() override;

private:
    Bounce_Teensy _bounce;
};

