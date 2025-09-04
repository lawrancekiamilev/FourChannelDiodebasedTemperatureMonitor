#ifndef PinDeff_h
#define PinDeff_h
//Librarys
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "pico/multicore.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include <math.h>
#include <time.h>
//Current Control Mux High = 10uA our Low = GND

#define CurrentConA0        0
#define CurrentConA1        1
//buttons
#define b1                  2
#define b2                  3
extern volatile uint32_t last_interrupt_time_button1;
extern volatile uint32_t last_interrupt_time_button2;
extern volatile bool b1Process;
extern volatile bool b2Process;
#define DEBOUNCE_TIME_MS 200 

//Channel LED indicatiors
#define Chan0LED            11
#define Chan1LED            9
#define Chan2LED            8
#define Chan3LED            6

// SPI Defines
#define SPI_PORT spi0
#define PIN_CS              21
#define PIN_SCK             18
#define PIN_MOSI            19
#define PIN_RST             20
#define PIN_DC              22

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA             4
#define I2C_SCL             5

//I2C Addresses
#define TCAADDR             0x70
#define EEPROM_ADDRESS      0x50 
#define BUFFER_SIZE         10
#define NUM_CHANNELS        4

//ADC specific Pin
#define ADC1RDY             8
#define ADC0RDY             13

//cuurentSense
#define ADC0                26
#define ioutControl         1

extern int CurrentMuxSleepTime;
extern mutex_t StopMem;
extern bool chanConn1[];
extern bool ChanStateChange[];
extern float nonIdealDiodeFactor;
// EEPROM arrays
typedef struct array {
    char ArrayNAME[17];
    char ArrayColor[17];
    char ArrayID[17];
    char ArrayFabDate[17];
} array_t;
extern array_t chan0array;
extern array_t chan1array;
extern array_t chan2array;
extern array_t chan3array;

extern bool chan0D; 
extern bool chan1D; 
extern bool chan2D; 
extern bool chan3D; 

extern uint8_t ArrayNAMEAdd;
extern uint8_t ArrayColorAdd;
extern uint8_t ArrayIDAdd;
extern uint8_t ArrayFabDateAdd;

extern bool chanConnected[];
extern bool PreChanConnected[];
extern float temp[];

//Samples avrage amount
extern int Samp;

extern volatile bool chanDiode;

//sleepVars
extern int StandSleep;
extern int i2CMuxSleep;
extern int CurrentMuxtoGnd;
extern int CurrentMuxtoPwr;

//Function Prototyps
void setup();
void CurrentOn(int chan);
void allCurrentOn();
void CurrentOff(int chan);
void FastCurrentOff(int chan);
void allCurrentOff();
void i2cChan(int chan);
void readStringFromEEPROM(int deviceAddress, uint8_t eepromAddress, char* buffer, int length);
void readEEPROM(int Chan);
bool isEEPROMConnected(int deviceAddress);
float voltToTempLakeShore(float voltage);
float voltToTempRicc(float voltage);
void CheckChan();
uint16_t readChan(int chan);
void core1_ent();
void PrintDisplay();
void PrintTemp(int x,int y, float temp,float Vh, float Vl);
void setupDiode();
void pickDiode(int chan);
float vlotToTempRiccnonideal(float voltage1, float voltage2);
void setChand10u(int chan, bool HighC);
void SetChannelLED(int chan);
float roundTemp(float num, int n);
#endif