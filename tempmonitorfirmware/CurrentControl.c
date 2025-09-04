#include "PinDeff.h"
void CurrentOn(int chan)
{
    switch (chan)
    {
    // 10uamp
    case 0:
        gpio_put(CurrentConA0, 0);
        gpio_put(CurrentConA1, 0);
        sleep_ms(CurrentMuxtoPwr);
        break;
    // 1uamp
    case 1:
        gpio_put(CurrentConA0, 0);
        gpio_put(CurrentConA1, 1);
        sleep_ms(CurrentMuxtoPwr);
        break;
    // gnd
    case 2:
        gpio_put(CurrentConA0, 1);
        gpio_put(CurrentConA1, 0);
        break;
    // float
    case 3:
        gpio_put(CurrentConA0, 1);
        gpio_put(CurrentConA1, 1);
        sleep_ms(CurrentMuxtoPwr);
        break;
    }
}
