#include <PCA9685.h>
#include <LedControl.h>
#include <Adafruit_MCP23017.h>
#include <array>
#include <vector>
#include <memory>
#include "Hardware/Button/MCPButton.h"
#include "Hardware/Button/MCPInterruptButton.h"
#include "Hardware/Button/TeensyButton.h"
#include "Hardware/Led/MCPLed.h"
#include "Hardware/Led/PCALed.h"
#include "Hardware/Slider/TeensySlider.h"

struct Peripherals
{
    Peripherals();

    void init(unsigned long startupDelay);
    void updateButtons();
    void clearLeds();
    void updateLeds();

    LedControl sld;
    Adafruit_MCP23017 mcp1;
    Adafruit_MCP23017 mcp2;
    PCA9685 pca1;
    PCA9685 pca2;

    std::array<MCPButton, 16> stepButtons;
    std::array<MCPButton, 4> instrumentButtons;
    MCPButton yellowButton;
    MCPButton redButton;
    MCPButton blueButton;
    MCPButton greenButton;

    std::array<PCALed, 16> redLeds;
    std::array<PCALed, 16> greenLeds;
    std::array<PCALed, 16>& blueLeds;
    std::array<MCPLed, 4> instrumentLeds;
    MCPLed yellowLed;
    MCPLed redLed;
    MCPLed blueLed;
    MCPLed greenLed;

    std::array<TeensySlider, 4> instrumentSliders;
    TeensySlider redSlider;
    TeensySlider blackSlider;
};