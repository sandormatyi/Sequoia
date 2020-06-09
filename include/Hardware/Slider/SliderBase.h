#pragma once

#include <inttypes.h>

class SliderBase 
{
public:
    virtual ~SliderBase() = default;

    virtual void init() = 0;
    virtual bool update() = 0;
    virtual float readNormalizedRawValue() const = 0;
};