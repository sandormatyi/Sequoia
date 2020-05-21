#include <array>
#include <Arduino.h>
#include <MIDI.h>
#include <Adafruit_MCP23017.h>
#include <PCA9685.h>
#include <LedControl.h>
#include "Hardware/Button/ButtonBase.h"
#include "Hardware/Led/LedBase.h"
#include "Hardware/Peripherals.h"
#include "Sequencer/Instrument.h"
#include "Sequencer/Sequencer.h"
#include "Hardware/Slider/TeensySlider.h"

#if defined(USBCON)
#include <midi_UsbTransport.h>

static const unsigned sUsbTransportBufferSize = 16;
typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;

UsbTransport sUsbTransport;

MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, MIDI);

#else // No USB available, fallback to Serial
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

#define _ENABLE_SERIAL 0
#if _ENABLE_SERIAL == 1
#define DBG Serial.printf
#else
int _dummyprintf(const char *, ...)
{
    return 0;
}
#define DBG _dummyprintf
#endif

Peripherals* p;
Sequencer* seq;

void printInstrumentInfo(uint8_t idx)
{
    const auto &instrument = seq->getInstrument(idx);
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

void printNoteInfo(uint8_t instrumentIdx, uint8_t noteIdx)
{
    const auto &note = seq->getInstrument(instrumentIdx).getNote(noteIdx);
    p->sld.clearDisplay(0);
    p->sld.setDigit(0, 7, note._noteNumber / 10, false);
    p->sld.setDigit(0, 6, note._noteNumber % 10, false);

    p->sld.setChar(0, 2, note._velocity / 100, false);
    p->sld.setChar(0, 1, (note._velocity % 100) / 10, false);
    p->sld.setChar(0, 0, note._velocity % 10, false);
}

void playStartupAnimation()
{
    constexpr unsigned long animationDelay = 100;

    for (uint8_t i = 0; i < 8; i++) {
        p->redLeds[i]->turnOn();
        p->redLeds[i]->update();
        p->sld.setDigit(0, i, i, false);
        delay(animationDelay);
    }

    p->channelSelectLed->turnOn();
    delay(animationDelay);
    p->barSelectLed->turnOn();
    delay(animationDelay);

    for (uint8_t i = 0; i < 8; i++) {
        p->blueLeds[7 - i]->turnOn();
        p->blueLeds[7 - i]->update();
        p->sld.setDigit(0, 7 - i, 8 + i, false);
        delay(animationDelay);
    }

    for (uint8_t i = 0; i < 8; ++i) {
        p->greenLeds[i]->turnOn();
        p->greenLeds[i]->update();
        p->sld.setChar(0, i, ' ', false);
        delay(animationDelay);
    }

    for (uint8_t fade = 0; fade < 10; ++fade) {
        const auto fadePercent = 100 - fade * 10;
        for (uint8_t i = 0; i < 8; ++i) {
            p->greenLeds[i]->turnOff();
            p->greenLeds[i]->update();
            p->redLeds[i]->setPWMValue(fadePercent);
            p->redLeds[i]->update();
            p->blueLeds[i]->setPWMValue(fadePercent);
            p->blueLeds[i]->update();
        }
        delay(animationDelay);
    }
}

void colorActiveNotes(uint8_t instrumentIdx, uint8_t barIdx) 
{
    const auto &instrument = seq->getInstrument(instrumentIdx);
    // uint8_t minNote = 255;
    // uint8_t maxNote = 0;
    // for (size_t i = 0; i < p->beatButtons.size(); ++i) {
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

    for (size_t i = 0; i < p->beatButtons.size(); ++i) {
        const auto idx = i + (barIdx * 8);
        if (instrument.isActiveNote(idx)) {
            const auto &note = instrument.getNote(idx);
            const auto diffFromMax = (note._noteNumber > maxNote) ? 0 : 
                                     (note._noteNumber < minNote) ? range * 2 :
                                      maxNote - note._noteNumber;

            const auto normalizedDiff = float(diffFromMax) / float(range * 2);
            const auto normalizedVelocity = float(note._velocity) / 127.0f;

            p->redLeds[i]->setPWMValue(uint8_t(100.0f * normalizedVelocity * (1.0f - normalizedDiff)));
            p->blueLeds[i]->setPWMValue(uint8_t(100.0f * normalizedVelocity * normalizedDiff));
        }
    }
}

volatile uint8_t currentBeat = 0;
uint8_t currentBar = 0;
uint16_t bpm = 120;

void playBeat(uint8_t beatNumber)
{
    DBG("Beat %d\n", beatNumber);
    for (const auto& note : seq->getNotes(beatNumber)) {
        DBG("\t%d sent\n", note._noteNumber);
        usbMIDI.sendNoteOn(note._noteNumber, note._velocity, note._channel);
        usbMIDI.sendNoteOff(note._noteNumber, note._velocity, note._channel);
    }
}

void playNextBeat()
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
    while (!Serial) ;
#endif
    DBG("Setup started\n");

    constexpr unsigned long startupDelay = 500;
    delay(startupDelay);

    Wire.begin();
    p = new Peripherals();
    p->init(startupDelay);
    DBG("Peripherals initialized\n");

    usbMIDI.setHandleRealTimeSystem(midiRealtimeCallback);
    MIDI.begin();
    DBG("MIDI handling initialized\n");

    seq = new Sequencer();
    DBG("Sequencer initialized\n");

    p->clearLeds();
    p->updateLeds();
    p->updateButtons();
    p->slider->update();
    playStartupAnimation();
    printInstrumentInfo(0);

    DBG("Setup done\n");
}

uint8_t currentInstrument = 0;
int editedNote = -1;

void loop()
{
    usbMIDI.read();

    const auto sliderUpdated = p->slider->update();
    p->updateButtons();

    const bool barSelect = p->barSelectButton->read() == LOW;
    const bool instrumentSelect = p->channelSelectButton->read() == LOW;
    const bool clearPressed = p->clearButton->read() == LOW;
    const bool positivePressed = p->positiveButton->fallingEdge();
    const bool negativePressed = p->negativeButton->fallingEdge();
    const bool mutePressed = p->muteButton->fallingEdge();
    const bool muteReleased = p->muteButton->risingEdge();

    for (size_t i = 0; i < p->beatButtons.size(); ++i) {
        if (p->beatButtons[i]->fallingEdge()) {
            DBG("Button %d pressed\n", i);
            if (barSelect) {
                currentBar = i / 4;
            } else if (instrumentSelect) {
                currentInstrument = i;
                printInstrumentInfo(currentInstrument);
            } else {
                const auto noteIdx = currentBar * 8 + i;
                auto &instrument = seq->getInstrument(currentInstrument);
                seq->getInstrument(currentInstrument).toggleNote(noteIdx);    
                if (instrument.isActiveNote(noteIdx)) {
                    editedNote = noteIdx;
                    printNoteInfo(currentInstrument, noteIdx);
                }
            }
        } else if (p->beatButtons[i]->risingEdge()) {
            if (barSelect) {
                ;
            } else if (instrumentSelect) {
                ;
            } else {
                editedNote = -1;
                printInstrumentInfo(currentInstrument);
            }
        }
    }
    if (clearPressed) {
        if (barSelect) {
            ;
        } else if (instrumentSelect) {
            seq->clearInstrument(currentInstrument);
            for (auto led : p->blueLeds) {
                led->turnOn();
                led->update();
            }
            return;
        } else if (editedNote > -1) {
            ;
        } else {
            seq->clearInstruments();
            for (auto led : p->blueLeds) {
                led->turnOn();
                led->update();
            }
        }
        return;
    }
    if (positivePressed || negativePressed) {
        const auto increment = positivePressed ? 1 : -1;
        if (barSelect) {
            ;
        } else if (instrumentSelect) {
            Note note = seq->getInstrument(currentInstrument).getDefaultNote();
            note._noteNumber += increment;
            seq->getInstrument(currentInstrument).setDefaultNote(note);
            printInstrumentInfo(currentInstrument);
        } else if (editedNote > -1) {
            Note note = seq->getInstrument(currentInstrument).getNote(editedNote);
            note._noteNumber += increment;
            seq->getInstrument(currentInstrument).setNote(editedNote, note);
            printNoteInfo(currentInstrument, editedNote);
        }
    }
    if (mutePressed) {
        if (barSelect) {
            ;
        } else if (instrumentSelect) {
            seq->muteInstrument(currentInstrument, true);
        } else if (editedNote > -1) {
            ;
        } else {
            seq->muteAllInstruments(true);
        }
    }
    if (muteReleased) {
        seq->muteAllInstruments(false);
    }
    if (sliderUpdated) {
        const auto newValue = p->slider->readLevel();
        if (barSelect) {
            ;
        } else if (instrumentSelect) {
            Note note = seq->getInstrument(currentInstrument).getDefaultNote();
            note._velocity = min(32 * newValue, 127);
            seq->getInstrument(currentInstrument).setDefaultNote(note);
            printInstrumentInfo(currentInstrument);
        } else if (editedNote > -1) {
            Note note = seq->getInstrument(currentInstrument).getNote(editedNote);
            note._velocity = min(32 * newValue, 127);
            seq->getInstrument(currentInstrument).setNote(editedNote, note);
            printNoteInfo(currentInstrument, editedNote);
        }
    }


    p->clearLeds();

    // Update beat LEDs
    if (barSelect) {
        for (size_t i = 0; i < p->beatButtons.size(); ++i) {
            if (i / 4 == currentBar) {
                p->redLeds[i]->turnOn();
            }
        }
    } else if (instrumentSelect) {
        p->blueLeds[currentInstrument]->turnOn();
    } else {
        colorActiveNotes(currentInstrument, currentBar);
        const auto currentBeatLed = (currentBeat + Instrument::s_beatNumber - 1) % Instrument::s_beatNumber - currentBar * 8;
        for (size_t i = 0; i < p->beatButtons.size(); ++i) {
            if (isPlaying && (currentBeatLed >= 0 && currentBeatLed < 8)) {
                p->greenLeds[currentBeatLed]->turnOn();
            }
            // if (seq->getInstrument(currentInstrument).isActiveNote(i + currentBar * 8)) {
            //     p->redLeds[i]->turnOn();
            // }
        }
    }

    // Update status LEDs
    if (barSelect) {
        p->barSelectLed->turnOn();
    } else if (instrumentSelect) {
        p->channelSelectLed->turnOn();
    } else {
        if (isPlaying) {
            if ((currentBeat + Instrument::s_beatNumber - 1) % 4 < 2) {
                p->barSelectLed->turnOn();
            }
        }
    }
    p->updateLeds();
}