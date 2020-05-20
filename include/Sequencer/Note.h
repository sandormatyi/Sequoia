#pragma once

#include <inttypes.h>
#include <Arduino.h>

class Note 
{
public:
    Note(uint8_t noteNumber, uint8_t velocity = 127, uint8_t channel = 0)
        : _noteNumber(noteNumber)
        , _velocity(velocity)
        , _channel(channel)
    {}

    Note() = default;
    Note(const Note& n) = default;

public:
    uint8_t _noteNumber;
    uint8_t _velocity;
    uint8_t _channel;
};