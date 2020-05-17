#pragma once

class ButtonBase
{
public:
    virtual ~ButtonBase() = default;

    virtual void init() {}
    virtual int update() = 0;
    virtual int read() = 0;
    virtual bool risingEdge() = 0;
    virtual bool fallingEdge() = 0;
};