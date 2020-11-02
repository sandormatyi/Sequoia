#pragma once

#include <Arduino.h>
#include <inttypes.h>

class Note
{
public:
  Note(uint8_t noteNumber, uint8_t velocity = 127)
      : _noteNumber(noteNumber)
      , _velocity(velocity)
  {
  }

  Note() = default;
  Note(const Note& n) = default;

public:
  uint8_t _noteNumber;
  uint8_t _velocity;
};