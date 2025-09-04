#include "PinDeff.h"

// sample amout
int Samp = 15;

bool chan0D = false;
bool chan1D = false;
bool chan2D = false;
bool chan3D = false;
// SleepDefs
int StandSleep = 10;
int i2CMuxSleep = 10;
int CurrentMuxSleepTime = 50; // in microseconds

float nonIdealDiodeFactor = 1.048;
// Channel Diode Select
volatile bool chanDiode = false;

// EEPROM arrays
array_t chan0array;
array_t chan1array;
array_t chan2array;
array_t chan3array;
array_t genarray;
uint8_t ArrayNAMEAdd = 0x00;
uint8_t ArrayColorAdd = 0x10;
uint8_t ArrayIDAdd = 0x20;
uint8_t ArrayFabDateAdd = 0x30;

bool chanConnected[] = {false, false, false, false};
bool PreChanConnected[] = {false, false, false, false};
float temp[] = {0.0f, 0.0f, 0.0f, 0.0f};
mutex_t StopMem;