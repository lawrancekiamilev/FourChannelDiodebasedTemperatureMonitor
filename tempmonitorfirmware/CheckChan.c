#include "PinDeff.h"
#include "ads1115.h"

void CheckChan()
{
    uint16_t adc_value = 0;
    for (int i = 0; i < 4; i++)
    {
        setChand10u(i, true);
        adc_value = readChan(i);
        if (adc_value > 500)
        {
            mutex_enter_blocking(&StopMem);
            chanConnected[i] = true;
            chanConn1[i] = true;
            mutex_exit(&StopMem);
        }
        else
        {
            mutex_enter_blocking(&StopMem);
            chanConnected[i] = false;
            chanConn1[i] = false;
            mutex_exit(&StopMem);
        }
    }
}
uint16_t readChan(int chan)
{
  uint16_t adc_value = 0;
  SetChannelLED(chan);
  setAdc(chan);
  adc_value = getConversion();
  return adc_value;
}