#include "PinDeff.h"

// Function to check if EEPROM is connected
bool isEEPROMConnected(int deviceAddress)
{
    uint8_t dummy = 0;
    int result = i2c_read_blocking(i2c0, deviceAddress, &dummy, 1, true);
    return result >= 0;
}

// Function to read a string from EEPROM and remove spaces
void readStringFromEEPROM(int deviceAddress, uint8_t eepromAddress, char *buffer, int length)
{
    // Read the string from EEPROM
    i2c_write_blocking(i2c0, deviceAddress, &eepromAddress, 1, true);
    i2c_read_blocking(i2c0, deviceAddress, buffer, length, true);
    buffer[length] = '\0'; // Null-terminate the string

    // Remove spaces from the string without removing other characters
    int j = 0;
    for (int i = 0; i < length; i++)
    {
        if (buffer[i] != ' ')
        {
            buffer[j++] = buffer[i];
        }
    }
    buffer[j] = '\0';
}

void readEEPROM(int Chan)
{
    if (Chan == 0)
    {
        gpio_put(Chan0LED, 1);
        i2cChan(6);
        if (!isEEPROMConnected(EEPROM_ADDRESS))
        {
            mutex_enter_blocking(&StopMem);
            strcpy(chan0array.ArrayNAME, "EIREGeneric");
            strcpy(chan0array.ArrayColor, "30nm");
            strcpy(chan0array.ArrayID, "00000000000");
            strcpy(chan0array.ArrayFabDate, "000000");
            mutex_exit(&StopMem);
        }
        else
        {
            mutex_enter_blocking(&StopMem);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayNAMEAdd, chan0array.ArrayNAME, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayColorAdd, chan0array.ArrayColor, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayIDAdd, chan0array.ArrayID, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayFabDateAdd, chan0array.ArrayFabDate, 16);
            mutex_exit(&StopMem);
        }
        gpio_put(Chan0LED, 0);
    }
    if (Chan == 1)
    {
        gpio_put(Chan1LED, 1);
        i2cChan(5);
        if (!isEEPROMConnected(EEPROM_ADDRESS))
        {
            mutex_enter_blocking(&StopMem);
            strcpy(chan1array.ArrayNAME, "EIREGeneric");
            strcpy(chan1array.ArrayColor, "30nm");
            strcpy(chan1array.ArrayID, "00000000000");
            strcpy(chan1array.ArrayFabDate, "000000");
            mutex_exit(&StopMem);
        }
        else
        {
            mutex_enter_blocking(&StopMem);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayNAMEAdd, chan1array.ArrayNAME, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayColorAdd, chan1array.ArrayColor, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayIDAdd, chan1array.ArrayID, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayFabDateAdd, chan1array.ArrayFabDate, 16);
            mutex_exit(&StopMem);
        }
        gpio_put(Chan1LED, 0);
    }
    if (Chan == 2)
    {
        gpio_put(Chan2LED, 1);
        i2cChan(2);
        if (!isEEPROMConnected(EEPROM_ADDRESS))
        {
            mutex_enter_blocking(&StopMem);
            strcpy(chan2array.ArrayNAME, "EIREGeneric");
            strcpy(chan2array.ArrayColor, "30nm");
            strcpy(chan2array.ArrayID, "00000000000");
            strcpy(chan2array.ArrayFabDate, "000000");
            mutex_exit(&StopMem);
        }
        else
        {
            mutex_enter_blocking(&StopMem);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayNAMEAdd, chan2array.ArrayNAME, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayColorAdd, chan2array.ArrayColor, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayIDAdd, chan2array.ArrayID, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayFabDateAdd, chan2array.ArrayFabDate, 16);
            mutex_exit(&StopMem);
        }
        gpio_put(Chan2LED, 0);
    }

    if (Chan == 3)
    {
        gpio_put(Chan3LED, 1);
        i2cChan(0);
        if (!isEEPROMConnected(EEPROM_ADDRESS))
        {
            mutex_enter_blocking(&StopMem);
            strcpy(chan3array.ArrayNAME, "EIREGeneric");
            strcpy(chan3array.ArrayColor, "30nm");
            strcpy(chan3array.ArrayID, "00000000000");
            strcpy(chan3array.ArrayFabDate, "000000");
            mutex_exit(&StopMem);
        }
        else
        {
            mutex_enter_blocking(&StopMem);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayNAMEAdd, chan3array.ArrayNAME, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayColorAdd, chan3array.ArrayColor, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayIDAdd, chan3array.ArrayID, 16);
            readStringFromEEPROM(EEPROM_ADDRESS, ArrayFabDateAdd, chan3array.ArrayFabDate, 16);
            mutex_exit(&StopMem);
        }
        gpio_put(Chan3LED, 0);
    }
}
