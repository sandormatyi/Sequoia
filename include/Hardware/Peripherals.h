#include <PCA9685.h>
#include <LedControl.h>
#include <Adafruit_MCP23017.h>
#include <array>
#include <vector>
#include <memory>

class ButtonBase;
class LedBase;
class SliderBase;

struct Peripherals
{
    Peripherals();

    void init(unsigned long startupDelay);
    void updateButtons();
    void clearLeds();
    void updateLeds();

    LedControl sld;
    Adafruit_MCP23017 mcp;
    PCA9685 pca;

    std::array<std::unique_ptr<ButtonBase>, 8> beatButtons;
    std::unique_ptr<ButtonBase> channelSelectButton;
    std::unique_ptr<ButtonBase> barSelectButton;
    std::unique_ptr<ButtonBase> clearButton;
    std::unique_ptr<ButtonBase> positiveButton;
    std::unique_ptr<ButtonBase> negativeButton;
    std::unique_ptr<ButtonBase> muteButton;

    std::array<std::unique_ptr<LedBase>, 8> redLeds;
    std::array<std::unique_ptr<LedBase>, 8> blueLeds;
    std::array<std::unique_ptr<LedBase>, 8> greenLeds;
    std::unique_ptr<LedBase> channelSelectLed;
    std::unique_ptr<LedBase> barSelectLed;

    std::unique_ptr<SliderBase> slider;

private:
    std::vector<std::reference_wrapper<ButtonBase>> _buttons;
    std::vector<std::reference_wrapper<LedBase>> _leds;
};