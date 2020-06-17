#include "Sequencer/CC.h"
#include <usb_midi.h>
#include "Hardware/Peripherals.h"
#include "DBG.h"

constexpr std::array<uint8_t, 2> CC::globalControls;
constexpr std::array<std::array<uint8_t, 2>, Sequencer::s_instrumentNumber> CC::instrumentControls;

void sendAllCCValues(Peripherals& p, Sequencer& seq)
{
    DBG("Resetting CC values...\n");
    usbMIDI.sendControlChange(CC::globalControls[0], round(p.redSlider1.readNormalizedRawValue() * 127), CC::s_globalCCChannel);
    usbMIDI.sendControlChange(CC::globalControls[1], round(p.redSlider2.readNormalizedRawValue() * 127), CC::s_globalCCChannel);

    for (size_t i = 0; i < p.instrumentSliders.size(); ++i) {
        for (size_t j = 0; j < Peripherals::s_numSliderRows; ++j) {
            const auto& instrument = seq.getInstrument(i);
            usbMIDI.sendControlChange(CC::instrumentControls[i][j], round(p.instrumentSliders[i][j].readNormalizedRawValue() * 127), instrument.getChannel());
        }
    }
}