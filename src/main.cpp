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
#include "Hardware/Slider/TeensySlider.h"
#include "DBG.h"
#include <glob.h>


Peripherals* p;
Sequencer* seq;

std::array<bool, 16> buttonStates;

void printInstrumentInfo(const Instrument& instrument)
{
    p->sld.clearDisplay(0);
    const auto& note = instrument.getDefaultNote();
    const char* name = instrument.getName();
    p->sld.setDigit(0, 7, note._noteNumber / 10, false);
    p->sld.setDigit(0, 6, note._noteNumber % 10, false);
    p->sld.setChar(0, 3, name[0], false);
    p->sld.setChar(0, 2, name[1], false);
    p->sld.setChar(0, 1, name[2], false);
    p->sld.setChar(0, 0, name[3], false);
}

void printNoteInfo(const Note& note)
{
    p->sld.clearDisplay(0);
    p->sld.setDigit(0, 7, note._noteNumber / 10, false);
    p->sld.setDigit(0, 6, note._noteNumber % 10, false);

    p->sld.setChar(0, 2, note._velocity / 100, false);
    p->sld.setChar(0, 1, (note._velocity % 100) / 10, false);
    p->sld.setChar(0, 0, note._velocity % 10, false);
}

void playStartupAnimation()
{
    constexpr unsigned long animationDelay = 200;

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

            p->redLeds[i].setPWMValue(uint8_t(100.0f * normalizedVelocity * (1.0f - normalizedDiff)));
            p->greenLeds[i].setPWMValue(uint8_t(100.0f * normalizedVelocity * normalizedDiff));
        }
    }
}

volatile uint8_t currentBeat = 0;
uint8_t currentBar = 0;
uint16_t bpm = 120;

static void playBeat(uint8_t beatNumber)
{
    // static std::vector<Note> previousNotes;
    // for (const auto& note: previousNotes) {
    //     usbMIDI.sendNoteOff(note._noteNumber, note._velocity, note._channel);
    // }

    // previousNotes.clear();

    DBG("Beat %d\n", beatNumber);
    for (const auto& note : seq->getNotes(beatNumber)) {
        DBG("\t%d sent\n", note._noteNumber);
        // previousNotes.push_back(note);
        usbMIDI.sendNoteOn(note._noteNumber, note._velocity, note._channel);
        usbMIDI.sendNoteOff(note._noteNumber, note._velocity, note._channel);
    }
}

static void playNextBeat()
{
    playBeat(currentBeat);
    currentBeat = (currentBeat + 1) % Instrument::s_beatNumber;
}

byte CLOCK = 248;
byte START = 250;
byte CONTINUE = 251;
byte STOP = 252;
volatile uint8_t clockCounter = 0;
volatile bool isPlaying = false;

void midiRealtimeCallback(uint8_t msg)
{
    //DBG("Midi RT message: %x\n", msg);
    if (msg == CLOCK) {
        clockCounter++;
        if (clockCounter % 6 == 0)
            playNextBeat();

        clockCounter = clockCounter % 24;
    }

    if (msg == START || msg == CONTINUE) {
        isPlaying = true;
        clockCounter = 0;
        currentBeat = 0;
        playNextBeat();
    }

    if (msg == STOP) {
        isPlaying = false;
        currentBeat = 0;
        usbMIDI.sendControlChange(123,0,0); // All notes off
    }
}

//IntervalTimer timer;

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
    DBG("Peripherals initialized, free RAM: %d\n", FreeRam());

    usbMIDI.setHandleRealTimeSystem(midiRealtimeCallback);
    DBG("MIDI handling initialized\n");

    seq = new Sequencer();
    DBG("Sequencer initialized, free RAM: %d\n", FreeRam());

    p->clearLeds();
    p->updateLeds();
    p->updateButtons();
    p->redSlider.update();
    p->yellowSlider.update();
    playStartupAnimation();
    printInstrumentInfo(seq->getCurrentInstrument());

    DBG("Setup done, free RAM: %d\n", FreeRam());
}

int editedNote = -1;

void loop()
{
    usbMIDI.read();

    // const auto sliderUpdated = p->slider->update();
    const auto yellowSliderUpdated = p->yellowSlider.update();
    const auto redSliderUpdated = p->redSlider.update();
    p->updateButtons();

    const bool clearPressed = p->yellowButton.read() == LOW;
    const bool positivePressed = p->redButton.fallingEdge();
    const bool negativePressed = p->blueButton.fallingEdge();
    const bool mutePressed = p->greenButton.fallingEdge();
    const bool muteReleased = p->greenButton.risingEdge();
    const bool muteMode = p->greenButton.read() == LOW;

    bool instrumentEditMode = false;
    for (auto& button : p->instrumentButtons)
        instrumentEditMode |= button.read() == LOW;

    // const bool clearPressed = false;
    // const bool positivePressed = false;
    // const bool negativePressed = false;
    // const bool mutePressed = false;
    // const bool muteReleased = false;
    // const bool muteMode = false;

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
    if (positivePressed || negativePressed) {
        const auto increment = positivePressed ? 1 : -1;
        if (instrumentEditMode) {
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getDefaultNote();
            note._noteNumber += increment;
            instrument.setDefaultNote(note);
            printInstrumentInfo(instrument);
        } else if (editedNote > -1) {
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getNote(editedNote);
            note._noteNumber += increment;
            instrument.setNote(editedNote, note);
            printNoteInfo(note);
        }
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
    if (redSliderUpdated) {
        if (instrumentEditMode) {
            const auto newPitch = round(p->redSlider.readNormalizedRawValue() * 24);
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getDefaultNote();
            note._noteNumber = 36u + newPitch;
            instrument.setDefaultNote(note);
            printNoteInfo(note);
        } else if (editedNote > -1) {
            const auto newPitch = round(p->redSlider.readNormalizedRawValue() * 24) - 12;
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getNote(editedNote);
            note._noteNumber = instrument.getDefaultNote()._noteNumber + newPitch;
            instrument.setNote(editedNote, note);
            printNoteInfo(note);
        } else {
            const auto value = round(p->redSlider.readNormalizedRawValue() * 127);
            const auto &instrument = seq->getCurrentInstrument();
            const auto channel = instrument.getDefaultNote()._channel;
            const auto cc = instrument.getCC(0);
            usbMIDI.sendControlChange(cc, value, channel);
        }
    }
    if (yellowSliderUpdated) {
        if (instrumentEditMode) {
            const auto newValue = round(p->yellowSlider.readNormalizedRawValue() * 127);
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getDefaultNote();
            note._velocity = newValue;
            instrument.setDefaultNote(note);
            printNoteInfo(note);
        } else if (editedNote > -1) {
            const auto newValue = round(p->yellowSlider.readNormalizedRawValue() * 127);
            auto &instrument = seq->getCurrentInstrument();
            Note note = instrument.getNote(editedNote);
            note._velocity = newValue;
            instrument.setNote(editedNote, note);
            printNoteInfo(note);
        } else {
            const auto value = round(p->yellowSlider.readNormalizedRawValue() * 127);
            const auto &instrument = seq->getCurrentInstrument();
            const auto channel = instrument.getDefaultNote()._channel;
            const auto cc = instrument.getCC(1);
            usbMIDI.sendControlChange(cc, value, channel);
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
    const auto currentBeatLed = (currentBeat + Instrument::s_beatNumber - 1) % Instrument::s_beatNumber - currentBar * 8;
    if (isPlaying && (currentBeatLed >= 0 && currentBeatLed < 16)) {
        p->greenLeds[currentBeatLed].turnOn();
    }


    // Update status LEDs
    if (isPlaying) {
        if ((currentBeat + Instrument::s_beatNumber - 1) % 4 < 2) {
            p->yellowLed.turnOn();
        }
    }
    p->updateLeds();
}