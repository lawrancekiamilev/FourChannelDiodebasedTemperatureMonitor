#include "PinDeff.h"

void i2cChan(int chan)
{
    uint8_t data = 1 << chan;
    i2c_write_blocking(i2c0, TCAADDR, &data, 1, false);
    sleep_ms(i2CMuxSleep);
}