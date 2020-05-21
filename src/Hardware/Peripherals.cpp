#include "Hardware/Peripherals.h"
#include "Hardware/Pins.h"
#include "Hardware/Button/TeensyButton.h"
#include "Hardware/Button/MCPButton.h"
#include "Hardware/Led/TeensyLed.h"
#include "Hardware/Led/MCPLed.h"
#include "Hardware/Led/PCALed.h"
#include "Hardware/Slider/TeensySlider.h"


constexpr unsigned long DebounceTime = 10; // 10 ms debounce

Peripherals::Peripherals()
    : sld(PIN_SLD_DIN, PIN_SLD_CLK, PIN_SLD_CS)
    , mcp()
    , pca(0x0, PCA9685_MODE_LED_DIRECT, 800.0)
    // Buttons
    , beatButtons{
        std::make_unique<MCPButton>(mcp, PIN_PB_1, DebounceTime),
        std::make_unique<MCPButton>(mcp, PIN_PB_2, DebounceTime),
        std::make_unique<MCPButton>(mcp, PIN_PB_3, DebounceTime),
        std::make_unique<MCPButton>(mcp, PIN_PB_4, DebounceTime),
        std::make_unique<MCPButton>(mcp, PIN_PB_5, DebounceTime),
        std::make_unique<MCPButton>(mcp, PIN_PB_6, DebounceTime),
        std::make_unique<MCPButton>(mcp, PIN_PB_7, DebounceTime),
        std::make_unique<MCPButton>(mcp, PIN_PB_8, DebounceTime)}
    , channelSelectButton(std::make_unique<TeensyButton>(PIN_PB_CS, DebounceTime))
    , barSelectButton(std::make_unique<TeensyButton>(PIN_PB_BS, DebounceTime))
    , clearButton(std::make_unique<TeensyButton>(PIN_PB_CLR, DebounceTime))
    , positiveButton(std::make_unique<TeensyButton>(PIN_PB_POS, DebounceTime))
    , negativeButton(std::make_unique<TeensyButton>(PIN_PB_NEG, DebounceTime))
    , muteButton(std::make_unique<TeensyButton>(PIN_PB_MUTE, DebounceTime))
    // Leds
    , redLeds {
        std::make_unique<PCALed>(pca, PCA9685_LED0, true),
        std::make_unique<PCALed>(pca, PCA9685_LED1, true),
        std::make_unique<PCALed>(pca, PCA9685_LED2, true),
        std::make_unique<PCALed>(pca, PCA9685_LED3, true),
        std::make_unique<PCALed>(pca, PCA9685_LED4, true),
        std::make_unique<PCALed>(pca, PCA9685_LED5, true),
        std::make_unique<PCALed>(pca, PCA9685_LED6, true),
        std::make_unique<PCALed>(pca, PCA9685_LED7, true)}
    , blueLeds {
        std::make_unique<PCALed>(pca, PCA9685_LED8, true),
        std::make_unique<PCALed>(pca, PCA9685_LED9, true),
        std::make_unique<PCALed>(pca, PCA9685_LED10, true),
        std::make_unique<PCALed>(pca, PCA9685_LED11, true),
        std::make_unique<PCALed>(pca, PCA9685_LED12, true),
        std::make_unique<PCALed>(pca, PCA9685_LED13, true),
        std::make_unique<PCALed>(pca, PCA9685_LED14, true),
        std::make_unique<PCALed>(pca, PCA9685_LED15, true)}
    , greenLeds {
        std::make_unique<MCPLed>(mcp, PIN_LED_1_G, true),
        std::make_unique<MCPLed>(mcp, PIN_LED_2_G, true),
        std::make_unique<MCPLed>(mcp, PIN_LED_3_G, true),
        std::make_unique<MCPLed>(mcp, PIN_LED_4_G, true),
        std::make_unique<MCPLed>(mcp, PIN_LED_5_G, true),
        std::make_unique<MCPLed>(mcp, PIN_LED_6_G, true),
        std::make_unique<MCPLed>(mcp, PIN_LED_7_G, true),
        std::make_unique<MCPLed>(mcp, PIN_LED_8_G, true)}
    , channelSelectLed(std::make_unique<TeensyLed>(PIN_LED_CS, true))
    , barSelectLed(std::make_unique<TeensyLed>(PIN_LED_BS, true))
    // Sliders
    , slider(std::make_unique<TeensySlider>(A3, 4))
{
    for (size_t i = 0; i < beatButtons.size(); ++i)
        _buttons.push_back(*beatButtons[i]);

    _buttons.push_back(*channelSelectButton);
    _buttons.push_back(*barSelectButton);
    _buttons.push_back(*clearButton);
    _buttons.push_back(*positiveButton);
    _buttons.push_back(*negativeButton);
    _buttons.push_back(*muteButton);

    for (size_t i = 0; i < redLeds.size(); ++i) {
        _leds.push_back(*redLeds[i]);
        _leds.push_back(*blueLeds[i]);
        _leds.push_back(*greenLeds[i]);
    }

    _leds.push_back(*channelSelectLed);
    _leds.push_back(*barSelectLed);
}

void Peripherals::init(unsigned long startupDelay)
{
    // Initialize leds by type to avoid voltage drop on startup
    pca.setup();
    for (auto& led : redLeds)
        led->init();

    for (auto& led : blueLeds)
        led->init();

    delay(startupDelay);

    mcp.begin();
    for (auto& led : greenLeds)
        led->init();

    delay(startupDelay);

    sld.shutdown(0, false);
    sld.setIntensity(0, 12); // sets brightness (0~15 possible values)
    sld.clearDisplay(0);
    sld.setScanLimit(0, 7);
    
    delay(startupDelay);

    channelSelectLed->init();
    barSelectLed->init();

    for (auto& button : _buttons)
        button.get().init();

    slider->init();
}

void Peripherals::updateButtons()
{
    for (auto& button : _buttons)
        button.get().update();
}

void Peripherals::clearLeds() 
{
    for (auto &led : _leds)
        led.get().turnOff();
}

void Peripherals::updateLeds()
{
    for (auto &led : _leds)
        led.get().update();
}