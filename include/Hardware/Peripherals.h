#include <PCA9685.h>
#include <LedControl.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_MCP3008.h>
#include <I2C_eeprom.h>
#include <array>
#include <vector>
#include <memory>
#include "Hardware/Button/MCPButton.h"
#include "Hardware/Button/MCPInterruptButton.h"
#include "Hardware/Button/TeensyButton.h"
#include "Hardware/Led/MCPLed.h"
#include "Hardware/Led/PCALed.h"
#include "Hardware/Slider/TeensySlider.h"
#include "Hardware/Slider/MCPSlider.h"

struct Peripherals
{
    enum SliderRows { Top = 0, Bottom = 1, s_numSliderRows };

    Peripherals();

    void init(unsigned long startupDelay);
    void updateButtons();
    void clearLeds();
    void updateLeds();

    LedControl sld;
    Adafruit_MCP23017 mcp1;
    Adafruit_MCP23017 mcp2;
    Adafruit_MCP23017 mcp3;
    Adafruit_MCP3008 adc;

    PCA9685 pca1;
    PCA9685 pca2;
    I2C_eeprom presetMemory;

    std::array<MCPButton, 16> stepButtons;
    std::array<MCPButton, 4> instrumentButtons;
    MCPButton yellowButton;
    MCPButton redButton;
    MCPButton blueButton;
    MCPButton greenButton;
    MCPButton saveButton;
    MCPButton loadButton;

    std::array<PCALed, 16> redLeds;
    std::array<PCALed, 16> greenLeds;
    std::array<PCALed, 16>& blueLeds;
    std::array<MCPLed, 4> instrumentLeds;
    MCPLed yellowLed;
    MCPLed redLed;
    MCPLed blueLed;
    MCPLed greenLed;

    std::array<std::array<MCPSlider, s_numSliderRows>, 4> instrumentSliders;
    TeensySlider redSlider1;
    TeensySlider redSlider2;
    TeensySlider blackSlider1;
    TeensySlider blackSlider2;
};