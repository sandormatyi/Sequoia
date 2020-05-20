#include <PCA9685.h>
#include <LedControl.h>
#include <Adafruit_MCP23017.h>
#include <array>

class ButtonBase;
class LedBase;
class SliderBase;

struct Peripherals
{
    Peripherals();
    ~Peripherals();

    void init(unsigned long startupDelay);
    void updateButtons();
    void clearLeds();
    void updateLeds();

    LedControl sld;
    Adafruit_MCP23017 mcp;
    PCA9685 pca;

    std::array<ButtonBase *, 8> beatButtons;
    ButtonBase *channelSelectButton;
    ButtonBase *barSelectButton;
    ButtonBase *clearButton;
    ButtonBase *positiveButton;
    ButtonBase *negativeButton;
    ButtonBase *muteButton;

    std::array<LedBase *, 8> redLeds;
    std::array<LedBase *, 8> blueLeds;
    std::array<LedBase *, 8> greenLeds;
    LedBase *channelSelectLed;
    LedBase *barSelectLed;

    SliderBase* slider;
};