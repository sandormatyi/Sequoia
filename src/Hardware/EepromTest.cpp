#include "Hardware/EepromTest.h"
#include <I2C_eeprom.h>

#define DISPLAY_DECIMAL
#define BLOCK_TO_LENGTH 10

void dumpEEPROM(I2C_eeprom &ee, uint16_t memoryAddress, uint16_t length)
{
#ifdef DISPLAY_DECIMAL
    Serial.print("\t  ");
#endif
#ifdef DISPLAY_HEX
    Serial.print("\t ");
#endif
    for (int x = 0; x < BLOCK_TO_LENGTH; x++)
    {
        if (x != 0)
        {
#ifdef DISPLAY_DECIMAL
            Serial.print("    ");
#endif
#ifdef DISPLAY_HEX
            Serial.print("   ");
#endif
        }
#ifdef DISPLAY_DECIMAL
        Serial.print(x);
#endif
#ifdef DISPLAY_HEX
        Serial.print(x, HEX);
#endif
    }
    Serial.println();

    // block to defined length
    memoryAddress = memoryAddress / BLOCK_TO_LENGTH * BLOCK_TO_LENGTH;
    length = (length + BLOCK_TO_LENGTH - 1) / BLOCK_TO_LENGTH * BLOCK_TO_LENGTH;

    byte b = ee.readByte(memoryAddress);
    for (unsigned int i = 0; i < length; i++)
    {
        char buf[6];
        if (memoryAddress % BLOCK_TO_LENGTH == 0)
        {
            if (i != 0)
            {
                Serial.println();
            }
#ifdef DISPLAY_DECIMAL
            sprintf(buf, "%05d", memoryAddress);
#endif
#ifdef DISPLAY_HEX
            sprintf(buf, "%04X", memoryAddress);
#endif
            Serial.print(buf);
            Serial.print(":\t");
        }
#ifdef DISPLAY_DECIMAL
        sprintf(buf, "%03d", b);
#endif
#ifdef DISPLAY_HEX
        sprintf(buf, "%02X", b);
#endif
        Serial.print(buf);
        b = ee.readByte(++memoryAddress);
        Serial.print("  ");
    }
    Serial.println();
}

void testEeprom(I2C_eeprom &ee)
{
    Serial.print("Demo I2C eeprom library ");
    Serial.print(I2C_EEPROM_VERSION);
    Serial.println("\n");

    Serial.println("\nTEST: determine size");
    auto start = micros();
    int size = ee.determineSize();
    auto diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    if (size > 0)
    {
        Serial.print("SIZE: ");
        Serial.print(size);
        Serial.println(" KB");
    }
    else if (size == 0)
    {
        Serial.println("WARNING: Can't determine eeprom size");
    }
    else
    {
        Serial.println("ERROR: Can't find eeprom\nstopped...");
        while (1)
            ;
    }

    Serial.println("\nTEST: 64 byte page boundary writeBlock");
    ee.setBlock(0, 0, 128);
    dumpEEPROM(ee, 0, 128);
    char data[] = "11111111111111111111";
    ee.writeBlock(60, (uint8_t *)data, 10);
    dumpEEPROM(ee, 0, 128);

    Serial.println("\nTEST: 64 byte page boundary setBlock");
    ee.setBlock(0, 0, 128);
    dumpEEPROM(ee, 0, 128);
    ee.setBlock(60, '1', 10);
    dumpEEPROM(ee, 0, 128);

    Serial.println("\nTEST: 64 byte page boundary readBlock");
    ee.setBlock(0, 0, 128);
    ee.setBlock(60, '1', 6);
    dumpEEPROM(ee, 0, 128);
    char ar[100];
    memset(ar, 0, 100);
    ee.readBlock(60, (uint8_t *)ar, 10);
    Serial.println(ar);

    Serial.println("\nTEST: write large string readback in small steps");
    ee.setBlock(0, 0, 128);
    char data2[] = "0000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999A";
    ee.writeBlock(10, (uint8_t *)&data2, sizeof(data2));
    dumpEEPROM(ee, 0, 128);
    for (int i = 0; i < 100; i++)
    {
        if (i % 10 == 0)
            Serial.println();
        Serial.print(' ');
        Serial.print(ee.readByte(10 + i));
    }
    Serial.println();

    Serial.println("\nTEST: check almost endofPage writeBlock");
    ee.setBlock(0, 0, 128);
    char data3[] = "6666";
    ee.writeBlock(60, (uint8_t *)&data3, sizeof(data3));
    dumpEEPROM(ee, 0, 128);

    // Serial.println();
    // Serial.print("\nI2C speed:\t");
    // Serial.println(16000/(16+2*TWBR));
    // Serial.print("TWBR:\t");
    // Serial.println(TWBR);
    // Serial.println();

    auto totals = 0;
    Serial.print("\nTEST: timing writeByte()\t");
    start = micros();
    ee.writeByte(10, 1);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    Serial.print("TEST: timing writeBlock(50)\t");
    start = micros();
    ee.writeBlock(10, (uint8_t *)&data2, 50);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    Serial.print("TEST: timing readByte()\t\t");
    start = micros();
    ee.readByte(10);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    Serial.print("TEST: timing readBlock(50)\t");
    start = micros();
    ee.readBlock(10, (uint8_t *)&data2, 50);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    Serial.print("TOTALS: ");
    Serial.println(totals);
    totals = 0;

    // same tests but now with a 5 millisec delay in between.
    delay(5);

    Serial.print("\nTEST: timing writeByte()\t");
    start = micros();
    ee.writeByte(10, 1);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    delay(5);

    Serial.print("TEST: timing writeBlock(50)\t");
    start = micros();
    ee.writeBlock(10, (uint8_t *)&data2, 50);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    delay(5);

    Serial.print("TEST: timing readByte()\t\t");
    start = micros();
    ee.readByte(10);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    delay(5);

    Serial.print("TEST: timing readBlock(50)\t");
    start = micros();
    int xx = ee.readBlock(10, (uint8_t *)&data2, 50);
    diff = micros() - start;
    Serial.print("TIME: ");
    Serial.println(diff);
    totals += diff;

    Serial.print("TOTALS: ");
    Serial.println(totals);
    totals = 0;

    // does it go well?
    Serial.println(xx);

    Serial.println("\tDone...");
}