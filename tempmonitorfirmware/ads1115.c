#include "PinDeff.h"
#include "ads1115.h"

uint8_t buffer[3];
int adcSel = 0;
void writeReg(uint8_t reg, uint16_t value)
{
  buffer[0] = reg;
  buffer[1] = value >> 8;
  buffer[2] = value & 0xFF;
  i2cChan(adcSel);
  i2c_write_blocking(i2c0, ADS1115_I2C_ADDR, buffer, 3, true);
}
uint16_t readReg(uint8_t reg)
{
  buffer[0] = reg;
  i2cChan(adcSel);
  i2c_write_blocking(i2c0, ADS1115_I2C_ADDR, buffer, 1, true);
  ;
  i2c_read_blocking(i2c0, ADS1115_I2C_ADDR, buffer, 2, true);
  ;
  return ((buffer[0] << 8) | buffer[1]);
}

int16_t getConversion()
{
  uint16_t res = 0;
  res = readReg(ADS1115_POINTER_CONVERSION) >> 0;
  if (res > 32767)
  {
    res = 0;
  }
  return (int16_t)res;
}

void setAdc(int mux)
{
  uint16_t config =
      ADS1115_REG_CONFIG_CQUE_1CONV |
      ADS1115_COMPARATOR_NONLATCHING |
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW |
      ADS1115_COMPARATOR_TRADITIONAL |
      ADS1115_MODE_SINGLE_SHOT |
      ADS1115_PGA_2_048 |
      ADS1115_RATE_128_SPS;

  switch (mux)
  {
  case 0:
    config |= ADS1115_MUX_DIFF_0_1;
    adcSel = 4;
    break;
  case 1:
    config |= ADS1115_MUX_DIFF_2_3;
    adcSel = 4;
    break;
  case 2:
    config |= ADS1115_MUX_DIFF_0_1;
    adcSel = 0;
    break;
  case 3:
    config |= ADS1115_MUX_DIFF_2_3;
    adcSel = 0;
    break;
  }
  config |= ADS1115_REG_CONFIG_OS_SINGLE;
  i2cChan(adcSel);
  writeReg(ADS1115_POINTER_CONFIGURATION, config);
  writeReg(ADS1115_POINTER_HI_THRESH, 0x8000);
  writeReg(ADS1115_POINTER_LO_THRESH, 0x0000);
}

float ads1115_raw_to_volts(int16_t counts)
{
  return counts * .000062502;
}

float round_to_sig_figs(float value, int sig_figs)
{
  if (value == 0)
    return 0;

  // Determine the order of magnitude of the value
  float scale = pow(10, sig_figs - 1 - (int)floor(log10(fabs(value))));

  // Scale, round, then unscale to achieve the desired precision
  return round(value * scale) / scale;
}

float ReadADC(int chan)
{
  uint16_t adc_value = 0;
  float volt;
  SetChannelLED(chan);
  adc_value = readChan(chan);
  volt = ads1115_raw_to_volts(adc_value);
  volt = round_to_sig_figs(volt, 5);
  return volt;
}

