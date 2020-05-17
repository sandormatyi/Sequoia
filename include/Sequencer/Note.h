#pragma once

#include <inttypes.h>
#include <Arduino.h>

class Note 
{
public:
    Note(uint8_t noteNumber, uint8_t velocity = 255, uint8_t channel = 0)
        : _noteNumber(noteNumber)
        , _velocity(velocity)
        , _channel(channel)
    {}

    Note(const Note& n) = default;

    uint8_t getNoteNumber() const { return _noteNumber; }
    uint8_t getVelocity() const { return _velocity; }
    uint8_t getChannel() const { return _channel; }

private:
    uint8_t _noteNumber;
    uint8_t _velocity;
    uint8_t _channel;
};