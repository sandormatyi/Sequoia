#include <array>

class ButtonBase;
class LedBase;
class Adafruit_MCP23017;
class PCA9685;

struct Peripherals
{
    Peripherals(Adafruit_MCP23017 &mcp, PCA9685 &pca);
    ~Peripherals();

    void init();
    void updateButtons();
    void clearLeds();
    void updateLeds();

    std::array<ButtonBase *, 8> beatButtons;
    ButtonBase *channelSelectButton;
    ButtonBase *barSelectButton;
    ButtonBase *clearButton;

    std::array<LedBase *, 8> redLeds;
    std::array<LedBase *, 8> blueLeds;
    std::array<LedBase *, 8> greenLeds;
    LedBase *channelSelectLed;
    LedBase *barSelectLed;
};