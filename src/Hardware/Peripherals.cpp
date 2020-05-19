#include "Hardware/Peripherals.h"
#include "Hardware/Pins.h"
#include "Hardware/Button/TeensyButton.h"
#include "Hardware/Button/MCPButton.h"
#include "Hardware/Led/TeensyLed.h"
#include "Hardware/Led/MCPLed.h"
#include "Hardware/Led/PCALed.h"


constexpr unsigned long DebounceTime = 10; // 10 ms debounce

Peripherals::Peripherals()
    : sld(PIN_SLD_DIN, PIN_SLD_CLK, PIN_SLD_CS)
    , mcp()
    , pca(0x0, PCA9685_MODE_LED_DIRECT, 800.0)
{
    beatButtons = {
        new MCPButton(mcp, PIN_PB_1, DebounceTime),
        new MCPButton(mcp, PIN_PB_2, DebounceTime),
        new MCPButton(mcp, PIN_PB_3, DebounceTime),
        new MCPButton(mcp, PIN_PB_4, DebounceTime),
        new MCPButton(mcp, PIN_PB_5, DebounceTime),
        new MCPButton(mcp, PIN_PB_6, DebounceTime),
        new MCPButton(mcp, PIN_PB_7, DebounceTime),
        new MCPButton(mcp, PIN_PB_8, DebounceTime)
    };

    channelSelectButton = new TeensyButton(PIN_PB_CS, DebounceTime);
    barSelectButton = new TeensyButton(PIN_PB_BS, DebounceTime);
    clearButton = new TeensyButton(PIN_PB_CLR, DebounceTime);

    redLeds = {
        new PCALed(pca, PCA9685_LED0, true),
        new PCALed(pca, PCA9685_LED1, true),
        new PCALed(pca, PCA9685_LED2, true),
        new PCALed(pca, PCA9685_LED3, true),
        new PCALed(pca, PCA9685_LED4, true),
        new PCALed(pca, PCA9685_LED5, true),
        new PCALed(pca, PCA9685_LED6, true),
        new PCALed(pca, PCA9685_LED7, true)
    };
    
   blueLeds = {
        new PCALed(pca, PCA9685_LED8, true),
        new PCALed(pca, PCA9685_LED9, true),
        new PCALed(pca, PCA9685_LED10, true),
        new PCALed(pca, PCA9685_LED11, true),
        new PCALed(pca, PCA9685_LED12, true),
        new PCALed(pca, PCA9685_LED13, true),
        new PCALed(pca, PCA9685_LED14, true),
        new PCALed(pca, PCA9685_LED15, true)
    };
    
    greenLeds = {
        new MCPLed(mcp, PIN_LED_1_G, true),
        new MCPLed(mcp, PIN_LED_2_G, true),
        new MCPLed(mcp, PIN_LED_3_G, true),
        new MCPLed(mcp, PIN_LED_4_G, true),
        new MCPLed(mcp, PIN_LED_5_G, true),
        new MCPLed(mcp, PIN_LED_6_G, true),
        new MCPLed(mcp, PIN_LED_7_G, true),
        new MCPLed(mcp, PIN_LED_8_G, true)
    };
    
    channelSelectLed = new TeensyLed(PIN_LED_CS, true);
    barSelectLed = new TeensyLed(PIN_LED_BS, true);
}

Peripherals::~Peripherals()
{
    for (auto b : beatButtons)
        delete b;

    delete channelSelectButton;
    delete barSelectButton;
    delete clearButton;

    for (auto l : redLeds)
        delete l;

    for (auto l : blueLeds)
        delete l;

    for (auto l : greenLeds)
        delete l;

    delete channelSelectLed;
    delete barSelectLed;
}

void Peripherals::init(unsigned long startupDelay)
{
    pca.setup();
    for (auto led : redLeds)
        led->init();

    for (auto led : blueLeds)
        led->init();

    delay(startupDelay);

    mcp.begin();
    for (auto led : greenLeds)
        led->init();

    delay(startupDelay);

    sld.shutdown(0, false);
    sld.setIntensity(0, 8); // sets brightness (0~15 possible values)
    sld.clearDisplay(0);
    sld.setScanLimit(0, 7);
    
    delay(startupDelay);

    channelSelectLed->init();
    barSelectLed->init();

    for (auto button : beatButtons)
        button->init();

    channelSelectButton->init();
    barSelectButton->init();
    clearButton->init();
}

void Peripherals::updateButtons()
{
    channelSelectButton->update();
    barSelectButton->update();
    clearButton->update();
    for (auto button : beatButtons)
        button->update();
}

void Peripherals::clearLeds() 
{
    for (auto &led : redLeds)
        led->turnOff();

    for (auto &led : blueLeds)
        led->turnOff();

    for (auto &led : greenLeds)
        led->turnOff();

    channelSelectLed->turnOff();
    barSelectLed->turnOff();
}

void Peripherals::updateLeds()
{
    for (auto &led : redLeds)
        led->update();

    for (auto &led : blueLeds)
        led->update();

    for (auto &led : greenLeds)
        led->update();

    channelSelectLed->update();
    barSelectLed->update();
}