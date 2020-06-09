#include <array>
#include <Arduino.h>
#include <MIDI.h>
#include <midi_UsbTransport.h>
#include <Adafruit_MCP23017.h>
#include <PCA9685.h>
#include <LedControl.h>
#include "Hardware/Button/ButtonBase.h"
#include "Hardware/Led/LedBase.h"
#include "Hardware/Peripherals.h"
#include "Sequencer/Instrument.h"
#include "Sequencer/Sequencer.h"
#include "Sequencer/PlayHead.h"
#include "Sequencer/CC.h"
#include "Hardware/Slider/TeensySlider.h"
#include "DBG.h"
#include <glob.h>

enum class SyncMode
{
    Midi = 0,
    Internal = 1
};
SyncMode syncMode = SyncMode::Internal;

Peripherals* p;
Sequencer* seq;
PlayHead* playHead;

std::array<bool, 16> buttonStates;

void printRandomizeInfo(const Scale scale, const float probability) 
{
    p->sld.clearDisplay(0);

    p->sld.setChar(0, 7, scaleNames[scale][0], false);
    p->sld.setChar(0, 6, scaleNames[scale][1], false);
    p->sld.setChar(0, 5, scaleNames[scale][2], false);

    const auto percent = round(probability * 100);
    p->sld.setChar(0, 2, percent / 100, false);
    p->sld.setChar(0, 1, (percent % 100) / 10, false);
    p->sld.setChar(0, 0, percent % 10, false);
}

void printInstrumentInfo(const Instrument& instrument)
{
    p->sld.clearDisplay(0);
    const auto& note = instrument.getDefaultNote();
    const char* name = instrument.getName();
    p->sld.setDigit(0, 7, note._channel, true);
    p->sld.setChar(0, 5, name[0], false);
    p->sld.setChar(0, 4, name[1], false);
    p->sld.setChar(0, 3, name[2], false);
    p->sld.setChar(0, 2, name[3], false);
    p->sld.setChar(0, 1, name[4], false);
    p->sld.setChar(0, 0, name[5], false);
}

void printNoteInfo(const Note& note)
{
    static constexpr char notes[] = {'C', 'C', 'D', 'D', 'E', 'F',
                                     'F', 'G', 'G', 'A', 'A', 'B'};
    static constexpr bool sharps[] = {false, true,  false, true,  false, false,
                                      true,  false, true,  false, true,  false};

    p->sld.clearDisplay(0);
    p->sld.setDigit(0, 7, note._noteNumber / 10, false);
    p->sld.setDigit(0, 6, note._noteNumber % 10, false);
    p->sld.setChar(0, 4, notes[note._noteNumber % 12], sharps[note._noteNumber % 12]);

    p->sld.setChar(0, 2, note._velocity / 100, false);
    p->sld.setChar(0, 1, (note._velocity % 100) / 10, false);
    p->sld.setChar(0, 0, note._velocity % 10, false);
}

void printBpmInfo(const double bpm)
{
    p->sld.clearDisplay(0);
    p->sld.setChar(0, 7, 'B', false);
    p->sld.setChar(0, 6, 'P', false);
    p->sld.setChar(0, 5, 'M', false);

    const auto intBpm = uint32_t(bpm);
    p->sld.setDigit(0, 2, intBpm / 100, false);
    p->sld.setDigit(0, 1, (intBpm % 100) / 10, false);
    p->sld.setDigit(0, 0, intBpm % 10, false);
}

void playStartupAnimation()
{
    constexpr unsigned long animationDelay = 100;

    for (uint8_t i = 0; i < 16; i++) {
        p->redLeds[i].turnOn();
        p->redLeds[i].update();
        p->greenLeds[15 - i].turnOn();
        p->greenLeds[15 - i].update();
        p->sld.setDigit(0, i / 2, i / 2, false);

        p->instrumentLeds[i / 4].turnOn();
        p->instrumentLeds[i / 4].update();
        switch(i / 4) {
        case 0:
            p->yellowLed.turnOn();
            p->yellowLed.update();
            break;
        case 1:
            p->redLed.turnOn();
            p->redLed.update();
            break;
        case 2:
            p->blueLed.turnOn();
            p->blueLed.update();
            break;
        case 3:
            p->greenLed.turnOn();
            p->greenLed.update();
            break;
        }

        delay(animationDelay);
    }
}

static void colorActiveNotes(Instrument& instrument, uint8_t barIdx)
{
    // uint8_t minNote = 255;
    // uint8_t maxNote = 0;
    // for (size_t i = 0; i < p->stepButtons.size(); ++i) {
    //     const auto idx = i + (barIdx * 8);
    //     if (instrument.isActiveNote(idx)) {
    //         const auto &note = instrument.getNote(idx);
    //         if (note._noteNumber < minNote)
    //             minNote = note._noteNumber;

    //         if (note._noteNumber > maxNote)
    //             maxNote = note._noteNumber;
    //     }
    // }
    // const auto range = maxNote - minNote;
    const auto &defaultNote = instrument.getDefaultNote();
    constexpr auto range = 12;
    const auto maxNote = defaultNote._noteNumber + range;
    const auto minNote = defaultNote._noteNumber - range;

    for (size_t i = 0; i < p->stepButtons.size(); ++i) {
        const auto idx = i + (barIdx * 16);
        if (instrument.isActiveNote(idx)) {
            const auto &note = instrument.getNote(idx);
            const auto diffFromMax = (note._noteNumber > maxNote) ? 0 :
                                     (note._noteNumber < minNote) ? range * 2 :
                                     maxNote - note._noteNumber;

            const auto normalizedDiff = float(diffFromMax) / float(range * 2);
            const auto normalizedVelocity = float(note._velocity) / 127.0f;

            p->redLeds[i].setPWMValue(normalizedVelocity * (1.0f - normalizedDiff));
            p->greenLeds[i].setPWMValue(normalizedVelocity * normalizedDiff);
        }
    }
}

uint8_t currentBar = 0;

void midiRealtimeCallback(uint8_t msg)
{
    constexpr byte CLOCK = 248;
    constexpr byte START = 250;
    constexpr byte CONTINUE = 251;
    constexpr byte STOP = 252;

    static uint8_t clockCounter = 0;

    //DBG("Midi RT message: %x\n", msg);
    if (msg == CLOCK) {
        clockCounter++;
        if (clockCounter % 6 == 0)
            playHead->step();
        clockCounter = clockCounter % 24;
    }

    if (msg == START || msg == CONTINUE) {
        clockCounter = 0;
        playHead->start(msg == START);
    }

    if (msg == STOP) {
        playHead->stop();
    }
}

IntervalTimer internalMetronome;
double bpm = 120.0;

void timerCallback()
{
    playHead->step();
}

void setup()
{
#if _ENABLE_SERIAL == 1
    Serial.begin(115200);
    while (!Serial && millis() < 5000) ;
#endif
    //DBG("Size of raw pointer: %d\n", sizeof(int*));
    //DBG("Size of std::unique_ptr: %d\n", sizeof(std::unique_ptr<int>));
    //DBG("Size of std::array: %d\n", sizeof(std::array<int, 1>));
    //DBG("Size of std::vector: %d\n", sizeof(std::vector<int>));
    DBG("Size of Peripherals: %d\n", sizeof(Peripherals));
    DBG("Size of Sequencer: %d\n", sizeof(Sequencer));

    DBG("Setup started, free RAM: %d\n", FreeRam());

    constexpr unsigned long startupDelay = 1000;
    //delay(startupDelay);

    Wire.begin();
    p = new Peripherals();
    p->init(startupDelay);
    randomSeed(analogRead(0));
    DBG("Peripherals initialized, free RAM: %d\n", FreeRam());

    usbMIDI.setHandleRealTimeSystem(midiRealtimeCallback);
    DBG("MIDI handling initialized\n");

    seq = new Sequencer();
    playHead = new PlayHead(*seq);
    DBG("Sequencer initialized, free RAM: %d\n", FreeRam());

    p->clearLeds();
    p->updateLeds();
    p->updateButtons();
    p->redSlider1.update();
    p->redSlider2.update();
    p->blackSlider1.update();
    p->blackSlider2.update();
    for (auto &sliderRow : p->instrumentSliders)
        for (auto &slider : sliderRow) 
            slider.update();

    playStartupAnimation();
    printInstrumentInfo(seq->getCurrentInstrument());

    DBG("Setup done, free RAM: %d\n", FreeRam());
}

int editedNote = -1;

void loop()
{
    usbMIDI.read();

    const auto blackSlider1Updated = p->blackSlider1.update();
    const auto blackSlider2Updated = p->blackSlider2.update();
    const auto redSlider1Updated = p->redSlider1.update();
    const auto redSlider2Updated = p->redSlider2.update();
    p->updateButtons();

    const bool clearPressed = p->yellowButton.read() == LOW;
    const bool mutePressed = p->greenButton.fallingEdge();
    const bool muteReleased = p->greenButton.risingEdge();
    const bool muteMode = p->greenButton.read() == LOW;
    const bool randomMode = p->redButton.read() == LOW;

    bool instrumentEditMode = false;
    for (auto& button : p->instrumentButtons)
        instrumentEditMode |= button.read() == LOW;

    for (size_t i = 0; i < p->stepButtons.size(); ++i) {
        if (p->stepButtons[i].fallingEdge()) {
            DBG("Button %d pressed\n", i);
            const auto noteIdx = currentBar * 8 + i;
            auto &instrument = seq->getCurrentInstrument();
            instrument.toggleNote(noteIdx);
            if (instrument.isActiveNote(noteIdx)) {
                editedNote = noteIdx;
                printNoteInfo(seq->getCurrentInstrument().getNote(noteIdx));
            }
        } else if (p->stepButtons[i].risingEdge()) {
            editedNote = -1;
            printInstrumentInfo(seq->getCurrentInstrument());
        }
    }
    for (size_t i = 0; i < p->instrumentButtons.size(); ++i) {
        if (p->instrumentButtons[i].fallingEdge()) {
            if (muteMode) {
                seq->muteInstrument(i, !seq->isInstrumentMuted(i));
            }

            seq->setCurrentInstrument(i);
            printInstrumentInfo(seq->getCurrentInstrument());
        } else if (p->instrumentButtons[i].risingEdge()) {
            printInstrumentInfo(seq->getCurrentInstrument());
        }
    }

    if (clearPressed) {
        if (instrumentEditMode) {
            seq->getCurrentInstrument().clear();
            for (auto& led : p->blueLeds) {
                led.turnOn();
                led.update();
            }
            return;
        } else if (editedNote > -1) {
            ;
        } else {
            seq->clearInstruments();
            for (auto& led : p->blueLeds) {
                led.turnOn();
                led.update();
            }
        }
        return;
    }
    if (p->blueButton.fallingEdge()) {
        if (playHead->isPlaying()) {
            playHead->stop();
            internalMetronome.end();
            syncMode = SyncMode::Midi;
            usbMIDI.setHandleRealTimeSystem(midiRealtimeCallback);
            playHead->stop();
        } else {
            syncMode = SyncMode::Internal;
            usbMIDI.setHandleRealTimeSystem(nullptr);
            playHead->start();
            const auto microsecondsPerStep = 1'000'000.0 / (bpm / 60.0 * 4.0);
            internalMetronome.begin(timerCallback, microsecondsPerStep);
        }
    } else if (p->blueButton.risingEdge()) {
        printInstrumentInfo(seq->getCurrentInstrument());
    }
    if (p->redButton.fallingEdge()) {
        const Scale scale = Scale(round(p->blackSlider1.readNormalizedRawValue() * 4));
        const auto probability = p->blackSlider2.readNormalizedRawValue();
        printRandomizeInfo(scale, probability);
    } else if (p->redButton.risingEdge()) {
        const Scale scale = Scale(round(p->blackSlider1.readNormalizedRawValue() * 4));
        const auto probability = p->blackSlider2.readNormalizedRawValue();
        seq->getCurrentInstrument().randomize(scale, probability);
        printInstrumentInfo(seq->getCurrentInstrument());
    }

    if (mutePressed) {
        if (instrumentEditMode) {
            seq->muteInstrument(seq->getCurrentInstrumentIdx(), true);
        } else if (editedNote > -1) {
            ;
        } else {
            seq->muteAllInstruments(true);
        }
    }
    if (muteReleased) {
        seq->muteAllInstruments(false);
    }
    if (blackSlider1Updated) {
        const auto sliderValue = p->blackSlider1.readNormalizedRawValue();
        if (randomMode) {
        const Scale scale = Scale(round(p->blackSlider1.readNormalizedRawValue() * 4));
            const auto probability = p->blackSlider2.readNormalizedRawValue();
            printRandomizeInfo(scale, probability);
        } else if (instrumentEditMode) {
            const auto newPitch = round(sliderValue * 24);
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getDefaultNote();
            note._noteNumber = 36u + newPitch;
            instrument.setDefaultNote(note);
            printNoteInfo(note);
        } else if (editedNote > -1) {
            const auto newPitch = round(sliderValue * 24) - 12;
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getNote(editedNote);
            note._noteNumber = instrument.getDefaultNote()._noteNumber + newPitch;
            instrument.setNote(editedNote, note);
            printNoteInfo(note);
        } else if (syncMode == SyncMode::Internal && p->blueButton.read() == LOW) {
            const auto value = sliderValue * 2.0;
            bpm = 60.0 * pow(2.0, value);
            DBG("BPM is %d\n", (int)bpm);
            const auto microsecondsPerStep = 1'000'000.0 / (bpm / 60.0 * 4.0);
            internalMetronome.update(microsecondsPerStep);
            printBpmInfo(bpm);
        }
    }
    if (blackSlider2Updated) {
        const auto sliderValue = p->blackSlider2.readNormalizedRawValue();
        if (randomMode) {
        const Scale scale = Scale(round(p->blackSlider1.readNormalizedRawValue() * 4));
            const auto probability = p->blackSlider2.readNormalizedRawValue();
            printRandomizeInfo(scale, probability);
        } else if (instrumentEditMode) {
            const auto newValue = round(sliderValue * 127);
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getDefaultNote();
            note._velocity = newValue;
            instrument.setDefaultNote(note);
            printNoteInfo(note);
        } else if (editedNote > -1) {
            const auto newValue = round(sliderValue * 127);
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getNote(editedNote);
            note._velocity = newValue;
            instrument.setNote(editedNote, note);
            printNoteInfo(note);
        }
    }
    if (redSlider1Updated) {
        const auto value = round(p->redSlider1.readNormalizedRawValue() * 127);
        const auto channel = CC::s_globalCCChannel;
        const auto cc = CC::globalControls[0];
        DBG("CC %d: %d\n", cc, value);
        usbMIDI.sendControlChange(cc, value, channel);
    }
    if (redSlider2Updated) {
        const auto value = round(p->redSlider2.readNormalizedRawValue() * 127);
        const auto channel = CC::s_globalCCChannel;
        const auto cc = CC::globalControls[1];
        DBG("CC %d: %d\n", cc, value);
        usbMIDI.sendControlChange(cc, value, channel);
    }

    for (size_t i = 0; i < p->instrumentSliders.size(); ++i) {
        for (size_t j = 0; j < Peripherals::s_numSliderRows; ++j) {
            if (p->instrumentSliders[i][j].update()) {
                const auto value = round(p->instrumentSliders[i][j].readNormalizedRawValue() * 127);
                const auto& instrument = seq->getInstrument(i);
                const auto channel = instrument.getDefaultNote()._channel;
                const auto ccNumber = CC::instrumentControls[i][j];
                DBG("CC %d: %d\n", ccNumber, value);
                usbMIDI.sendControlChange(ccNumber, value, channel);
            }
        }
           
    }

    p->clearLeds();

    // Update instrument LEDs
    if (muteMode) {
        for (size_t i = 0; i < p->instrumentButtons.size(); ++i) {
            if (seq->isInstrumentMuted(i)) {
                p->instrumentLeds[i].turnOn();
            }
        }
    } else {
        p->instrumentLeds[seq->getCurrentInstrumentIdx()].turnOn();
    }

    // Update step LEDs
    colorActiveNotes(seq->getCurrentInstrument(), currentBar);
    const auto currentStepLed = playHead->getCurrentStep() - currentBar * 8;
    if (playHead->isPlaying() && (currentStepLed >= 0 && currentStepLed < 16)) {
        p->greenLeds[currentStepLed].turnOn();
    }

    // Update status LEDs
    if (playHead->isPlaying()) {
        if (playHead->getCurrentStep() % 4 < 2) {
            p->blueLed.turnOn();
        }
    }
    if (muteMode) {
        p->greenLed.turnOn();
    }
    if (randomMode) {
        p->redLed.turnOn();
    }

    p->updateLeds();
}