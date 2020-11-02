#pragma once

#include <inttypes.h>

class SliderBase
{
public:
  virtual ~SliderBase() = default;

  virtual void init() = 0;
  virtual bool update() = 0;
  virtual float readNormalizedRawValue() const = 0;

protected:
  static float mapToDeadBand(float value, const float maxValue, const float deadband)
  {
    value = constrain(value, deadband, maxValue - deadband);
    value = map(value, deadband, maxValue - deadband, 0, maxValue);
    return value;
  }
};