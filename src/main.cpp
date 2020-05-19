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
    const auto& note = instrument.getNote();
    const char* name = instrument.getName();
    p->sld.setDigit(0, 7, note.getNoteNumber() / 10, false);
    p->sld.setDigit(0, 6, note.getNoteNumber() % 10, false);
    p->sld.setChar(0, 3, name[0], false);
    p->sld.setChar(0, 2, name[1], false);
    p->sld.setChar(0, 1, name[2], false);
    p->sld.setChar(0, 0, name[3], false);
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
}

volatile uint8_t currentBeat = 0;
uint8_t currentBar = 0;
uint16_t bpm = 120;

void playBeat(uint8_t beatNumber)
{
    DBG("Beat %d\n", beatNumber);
    for (const auto& note : seq->getNotes(beatNumber)) {
        DBG("\t%d sent\n", note.getNoteNumber());
        usbMIDI.sendNoteOn(note.getNoteNumber(), note.getVelocity(), note.getChannel());
        usbMIDI.sendNoteOff(note.getNoteNumber(), note.getVelocity(), note.getChannel());
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
    playStartupAnimation();
    printInstrumentInfo(0);

    DBG("Setup done\n");
}

uint8_t currentInstrument = 0;

void loop()
{
    p->updateButtons();

    const bool barSelect = p->barSelectButton->read() == LOW;
    const bool instrumentSelect = p->channelSelectButton->read() == LOW;
    const bool clear = p->clearButton->read() == LOW;
    if (clear) {
        seq->clearInstruments();
        for (auto led : p->blueLeds) {
            led->turnOn();
            led->update();
        }
        return;
    }

    for (size_t i = 0; i < p->beatButtons.size(); ++i) {
        if (p->beatButtons[i]->fallingEdge()) {
            DBG("Button %d pressed\n", i);
            if (barSelect) {
                currentBar = i / 4;
            } else if (instrumentSelect) {
                currentInstrument = i;
                printInstrumentInfo(i);
            } else {
                seq->getInstrument(currentInstrument).toggleBeat(currentBar * 8 + i);
            }
        }
    }

    p->clearLeds();

    // Update beat LEDs
    for (size_t i = 0; i < p->beatButtons.size(); ++i) {
        if (barSelect) {
            if (i / 4 == currentBar) {
                p->redLeds[i]->turnOn();
            }
        } else if (instrumentSelect) {
            if (i == currentInstrument) {
                p->blueLeds[i]->turnOn();
            }
        } else {
            if (i == currentInstrument) {
                p->blueLeds[i]->setPWMValue(25);
            }
            if (isPlaying && currentBeat >= currentBar * 8 && currentBeat < (currentBar + 1) * 8) {
                p->greenLeds[currentBeat - currentBar * 8]->turnOn();
            }
            if (seq->getInstrument(currentInstrument).isBeatSet(i + currentBar * 8)) {
                p->redLeds[i]->turnOn();
            }
        }
    }

    // Update status LEDs
    if (barSelect) {
        p->barSelectLed->turnOn();
    } else if (instrumentSelect) {
        p->channelSelectLed->turnOn();
    } else {
        if (isPlaying) {
            if ((currentBeat % 4) > 1) {
                p->barSelectLed->turnOn();
            }
        }
    }
    p->updateLeds();

    usbMIDI.read();
}