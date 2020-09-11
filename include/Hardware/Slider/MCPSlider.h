#pragma once

#include "SliderBase.h"
#include <Adafruit_MCP3008.h>

class MCPSlider : public SliderBase
{
public:
    MCPSlider(Adafruit_MCP3008& adc, uint8_t channel);
    virtual ~MCPSlider() = default;

    void init() override {}
    bool update() override;

    float readNormalizedRawValue() const override;

private:
    float getNormalizedValue(int value) const;

    static constexpr int s_maxValue = 1023;
    static constexpr int s_deadband = 24;
    static constexpr int s_tolerance = 16;
    Adafruit_MCP3008& _adc;
    uint8_t _channel;
    uint16_t _value;
};