#include "Hardware/Peripherals.h"
#include "Hardware/Pins.h"
#include "Hardware/Button/TeensyButton.h"
#include "Hardware/Button/MCPButton.h"
#include "Hardware/Button/MCPInterruptButton.h"
#include "Hardware/Led/TeensyLed.h"
#include "Hardware/Led/MCPLed.h"
#include "Hardware/Led/PCALed.h"
#include "Hardware/Slider/TeensySlider.h"
#include "DBG.h"

void mcpInterruptCallback() 
{
    //mcpThis->readGPIOAB();
    //uint8_t pin = mcpThis->getLastInterruptPin();
    //DBG("Interrupt from MCP pin %d\n", pin);
    //attachInterrupt(20, mcpInterruptCallback, FALLING);
}

constexpr unsigned long DebounceTime = 10; // 10 ms debounce

Peripherals::Peripherals()
    : sld(PIN_SLD_DIN, PIN_SLD_CLK, PIN_SLD_CS)
    , mcp1()
    , mcp2()
    , pca1(0x0, PCA9685_MODE_LED_DIRECT, 800.0)
    , pca2(0x1, PCA9685_MODE_LED_DIRECT, 800.0)
    // Buttons
    , stepButtons {
        MCPButton(mcp1, PIN_PB_1, DebounceTime),
        MCPButton(mcp1, PIN_PB_2, DebounceTime),
        MCPButton(mcp1, PIN_PB_3, DebounceTime),
        MCPButton(mcp1, PIN_PB_4, DebounceTime),
        MCPButton(mcp1, PIN_PB_5, DebounceTime),
        MCPButton(mcp1, PIN_PB_6, DebounceTime),
        MCPButton(mcp1, PIN_PB_7, DebounceTime),
        MCPButton(mcp1, PIN_PB_8, DebounceTime),
        MCPButton(mcp1, PIN_PB_9, DebounceTime),
        MCPButton(mcp1, PIN_PB_10, DebounceTime),
        MCPButton(mcp1, PIN_PB_11, DebounceTime),
        MCPButton(mcp1, PIN_PB_12, DebounceTime),
        MCPButton(mcp1, PIN_PB_13, DebounceTime),
        MCPButton(mcp1, PIN_PB_14, DebounceTime),
        MCPButton(mcp1, PIN_PB_15, DebounceTime),
        MCPButton(mcp1, PIN_PB_16, DebounceTime)}
    , instrumentButtons {
        MCPButton(mcp2, PIN_PB_INST_1, DebounceTime),
        MCPButton(mcp2, PIN_PB_INST_2, DebounceTime),
        MCPButton(mcp2, PIN_PB_INST_3, DebounceTime),
        MCPButton(mcp2, PIN_PB_INST_4, DebounceTime)}
    , yellowButton(mcp2, PIN_PB_YELLOW, DebounceTime)
    , redButton(mcp2, PIN_PB_RED, DebounceTime)
    , blueButton(mcp2, PIN_PB_BLUE, DebounceTime)
    , greenButton(mcp2, PIN_PB_GREEN, DebounceTime)
    // Leds
    , redLeds {
        PCALed(pca1, PCA9685_LED0, true),
        PCALed(pca1, PCA9685_LED1, true),
        PCALed(pca1, PCA9685_LED2, true),
        PCALed(pca1, PCA9685_LED3, true),
        PCALed(pca1, PCA9685_LED4, true),
        PCALed(pca1, PCA9685_LED5, true),
        PCALed(pca1, PCA9685_LED6, true),
        PCALed(pca1, PCA9685_LED7, true),
        PCALed(pca2, PCA9685_LED0, true),
        PCALed(pca2, PCA9685_LED1, true),
        PCALed(pca2, PCA9685_LED2, true),
        PCALed(pca2, PCA9685_LED3, true),
        PCALed(pca2, PCA9685_LED4, true),
        PCALed(pca2, PCA9685_LED5, true),
        PCALed(pca2, PCA9685_LED6, true),
        PCALed(pca2, PCA9685_LED7, true)}
    , greenLeds {
        PCALed(pca1, PCA9685_LED8, true),
        PCALed(pca1, PCA9685_LED9, true),
        PCALed(pca1, PCA9685_LED10, true),
        PCALed(pca1, PCA9685_LED11, true),
        PCALed(pca1, PCA9685_LED12, true),
        PCALed(pca1, PCA9685_LED13, true),
        PCALed(pca1, PCA9685_LED14, true),
        PCALed(pca1, PCA9685_LED15, true),
        PCALed(pca2, PCA9685_LED8, true),
        PCALed(pca2, PCA9685_LED9, true),
        PCALed(pca2, PCA9685_LED10, true),
        PCALed(pca2, PCA9685_LED11, true),
        PCALed(pca2, PCA9685_LED12, true),
        PCALed(pca2, PCA9685_LED13, true),
        PCALed(pca2, PCA9685_LED14, true),
        PCALed(pca2, PCA9685_LED15, true)}
    , blueLeds(redLeds)
    // , blueLeds {
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_1_B, true),
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_2_B, true),
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_3_B, true),
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_4_B, true),
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_5_B, true),
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_6_B, true),
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_7_B, true),
    //     std::make_unique<MCPLed>(mcp1, PIN_LED_8_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_9_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_10_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_11_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_12_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_13_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_14_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_15_B, true),
    //     std::make_unique<MCPLed>(mcp2, PIN_LED_16_B, true)}
    , instrumentLeds{
        MCPLed(mcp2, PIN_LED_INST_1),
        MCPLed(mcp2, PIN_LED_INST_2),
        MCPLed(mcp2, PIN_LED_INST_3),
        MCPLed(mcp2, PIN_LED_INST_4)}
    , yellowLed(mcp2, PIN_LED_YELLOW)
    , redLed(mcp2, PIN_LED_RED)
    , blueLed(mcp2, PIN_LED_BLUE)
    , greenLed(mcp2, PIN_LED_GREEN)
    // Sliders
    , redSlider(PIN_POT_RED, 32)
    , yellowSlider(PIN_POT_YELLOW, 32)
{
    DBG("Size of MCP: %d\n", sizeof(Adafruit_MCP23017));
    DBG("Size of PCA: %d\n", sizeof(PCA9685));
    DBG("Size of SLD: %d\n", sizeof(LedControl));

    DBG("Size of TeensyLed: %d\n", sizeof(TeensyLed));
    DBG("Size of MCPLed: %d\n", sizeof(MCPLed));
    DBG("Size of PCALed: %d\n", sizeof(PCALed));
    DBG("Size of TeensyButton: %d\n", sizeof(TeensyButton));
    DBG("Size of MCPButton: %d\n", sizeof(MCPButton));
    DBG("Size of MCPInterruptButton: %d\n", sizeof(MCPInterruptButton));

    //mcpThis = &mcp1;
}

void Peripherals::init(unsigned long startupDelay)
{
    delay(startupDelay);

    sld.shutdown(0, false);
    sld.setIntensity(0, 12); // sets brightness (0~15 possible values)
    sld.clearDisplay(0);
    sld.setScanLimit(0, 7);

    delay(startupDelay);

    pca1.setup();
    pca2.setup();

    mcp1.begin(0);
    mcp2.begin(1);

    // mcp1.setupInterrupts(true, false, LOW);
    // pinMode(20, INPUT);
    // attachInterrupt(20, mcpInterruptCallback, FALLING);

    for (auto& led : redLeds)
        led.init();

    for (auto& led : greenLeds)
        led.init();

    //for (auto& led : blueLeds)
    //    led.init();

    for (auto& led : instrumentLeds)
        led.init();

    yellowLed.init();
    redLed.init();
    blueLed.init();
    greenLed.init();

    for (auto& button: stepButtons)
        button.init();

    for (auto& button: instrumentButtons)
        button.init();

    yellowButton.init();
    redButton.init();
    blueButton.init();
    greenButton.init();

    redSlider.init();
    yellowSlider.init();
}

void Peripherals::updateButtons()
{
    for (auto& button: stepButtons)
        button.update();

    for (auto& button: instrumentButtons)
        button.update();

    yellowButton.update();
    redButton.update();
    blueButton.update();
    greenButton.update();
}

void Peripherals::clearLeds() 
{
    for (auto& led : redLeds)
        led.turnOff();

    for (auto& led : greenLeds)
        led.turnOff();

    //for (auto& led : blueLeds)
    //    led.turnOff();

    for (auto& led : instrumentLeds)
        led.turnOff();

    yellowLed.turnOff();
    redLed.turnOff();
    blueLed.turnOff();
    greenLed.turnOff();
}

void Peripherals::updateLeds()
{
    for (auto& led : redLeds)
        led.update();

    for (auto& led : greenLeds)
        led.update();

    //for (auto& led : blueLeds)
    //    led.update();

    for (auto& led : instrumentLeds)
        led.update();

    yellowLed.update();
    redLed.update();
    blueLed.update();
    greenLed.update();
}