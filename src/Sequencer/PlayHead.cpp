#include "Sequencer/PlayHead.h"
#include "Sequencer/Sequencer.h"
#include <usb_midi.h>

PlayHead::PlayHead(Sequencer &sequencer)
    : _sequencer(sequencer)
    , _nextStep(0)
    , _isPlaying(false)
{
}

void PlayHead::start(bool fromBeginning = true)
{
    _isPlaying = true;
    if (fromBeginning)
        _nextStep = 0;

    step();
}

void PlayHead::stop()
{
    _isPlaying = false;
    usbMIDI.sendControlChange(123, 0, 0); // All notes off
    _nextStep = 0;
}

void PlayHead::step()
{
    playStep(_nextStep);

    const auto maxSteps = Instrument::s_stepNumber;
    _nextStep = (_nextStep + 1) % maxSteps;
}

uint8_t PlayHead::getCurrentStep() const
{
    return (_nextStep - 1 + Instrument::s_stepNumber) % Instrument::s_stepNumber;
}

bool PlayHead::isPlaying() const
{
    return _isPlaying;
}

void PlayHead::playStep(uint8_t stepIdx)
{
    // static std::vector<Note> previousNotes;
    // for (const auto& note: previousNotes) {
    //     usbMIDI.sendNoteOff(note._noteNumber, note._velocity, note._channel);
    // }

    // previousNotes.clear();

    DBG("Step %d\n", stepIdx);
    for (const auto &note : _sequencer.getNotes(stepIdx)) {
        DBG("\t%d sent\n", note._noteNumber);
        // previousNotes.push_back(note);
        usbMIDI.sendNoteOn(note._noteNumber, note._velocity, note._channel);
        usbMIDI.sendNoteOff(note._noteNumber, note._velocity, note._channel);
    }
}