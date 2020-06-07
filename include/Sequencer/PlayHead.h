#pragma once

#include <inttypes.h>
#include "DBG.h"

class Sequencer;

class PlayHead {
public:
    PlayHead(Sequencer &sequencer);

    void start(bool fromBeginning = true);
    void stop();
    void step();

    uint8_t getCurrentStep() const;
    bool isPlaying() const;

private:
    void playStep(uint8_t stepIdx);

    Sequencer &_sequencer;
    uint8_t _nextStep;
    bool _isPlaying;
};