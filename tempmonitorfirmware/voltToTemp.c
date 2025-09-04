#include "PinDeff.h"
float voltageVal[] = {
    0.090570, 0.110239, 0.136555, 0.179181, 0.265393, 0.349522, 0.452797, 0.513393,
    0.563128, 0.607845, 0.648723, 0.686936, 0.722511, 0.755487, 0.786992, 0.817025,
    0.844538, 0.869583, 0.893230, 0.914469, 0.934356, 0.952903, 0.970134, 0.986073,
    0.998925, 1.01064, 1.02125, 1.03167, 1.04189, 1.05192, 1.06277, 1.07472,
    1.09110, 1.09602, 1.10014, 1.10393, 1.10702, 1.10974, 1.11204, 1.11414,
    1.11628, 1.11853, 1.12090, 1.12340, 1.12589, 1.12913, 1.13494, 1.14495,
    1.16297, 1.17651, 1.19475, 1.24208, 1.26122, 1.27811, 1.29430, 1.31070,
    1.32727, 1.34506, 1.36423, 1.38361, 1.40454, 1.42732, 1.45206, 1.48578,
    1.53523, 1.56684, 1.58358, 1.59690, 1.60756, 1.62125, 1.62945, 1.63516,
    1.63943, 1.64261, 1.64430
};


float temperature[] = {
    500.0, 491.0, 479.5, 461.5, 425.5, 390.0, 346.0, 320.0, 
    298.5, 279.0, 261.0, 244.0, 228.0, 213.0, 198.5, 184.5, 
    171.5, 159.5, 148.0, 137.5, 127.5, 118.0, 109.0, 100.5, 
    93.5, 87.0, 81.0, 75.0, 69.0, 63.0, 56.4, 49.0, 
    38.7, 35.7, 33.3, 31.2, 29.6, 28.3, 27.3, 26.5, 
    25.8, 25.2, 24.7, 24.3, 24.0, 23.7, 23.3, 22.8, 
    22.0, 21.3, 20.2, 17.1, 15.9, 14.9, 14.0, 13.15, 
    12.35, 11.55, 10.75, 10.0, 9.25, 8.5, 7.75, 6.8, 
    5.46, 4.56, 4.04, 3.58, 3.18, 2.62, 2.26, 1.98, 
    1.74, 1.53, 1.4
};


float voltToTempLakeShore(float voltage) {
    for (int i = 1; i < 75; i++) { 
        if (voltage > voltageVal[i - 1] && voltage <= voltageVal[i]) {
            float slope = (temperature[i] - temperature[i-1]) / (voltageVal[i] - voltageVal[i-1]);
            float intercept = temperature[i] - slope*voltageVal[i];
            float temp = slope * voltage + intercept;
            return temp;
        }
        
    }
}
void SetChannelLED(int chan){
    switch(chan){
        case 0:
            gpio_put(Chan0LED, 1);
            gpio_put(Chan1LED, 0);
            gpio_put(Chan2LED, 0);
            gpio_put(Chan3LED, 0);
            break;
        case 1:
            gpio_put(Chan0LED, 0);
            gpio_put(Chan1LED, 1);
            gpio_put(Chan2LED, 0);
            gpio_put(Chan3LED, 0);
            break;
        case 2:
            gpio_put(Chan0LED, 0);
            gpio_put(Chan1LED, 0);
            gpio_put(Chan2LED, 1);
            gpio_put(Chan3LED, 0);
            break;
        case 3:
            gpio_put(Chan0LED, 0);
            gpio_put(Chan1LED, 0);
            gpio_put(Chan2LED, 0);
            gpio_put(Chan3LED, 1);
            break;
    }

}
void setChand10u(int chan, bool HighC) {
    if (HighC) {
        switch (chan)
        {
        case 0:
            gpio_put(CurrentConA0, 0);
            gpio_put(CurrentConA1, 0);
            break;
        case 1:
            gpio_put(CurrentConA0, 1);
            gpio_put(CurrentConA1, 0);
            break;
        case 2:
            gpio_put(CurrentConA0, 0);
            gpio_put(CurrentConA1, 1);
            break;
        case 3:
            gpio_put(CurrentConA0, 1);
            gpio_put(CurrentConA1, 1);
            break;
        
        default:
            break;
        }
    } else if (!HighC) {
        switch (chan)
        {
        case 0:
            gpio_put(CurrentConA0, 1);
            gpio_put(CurrentConA1, 1);
            break;
        case 1:
            gpio_put(CurrentConA0, 0);
            gpio_put(CurrentConA1, 1);
            break;
        case 2:
            gpio_put(CurrentConA0, 1);
            gpio_put(CurrentConA1, 0);
            break;
        case 3:
            gpio_put(CurrentConA0, 0);
            gpio_put(CurrentConA1, 0);
            break;
        
        default:
            break;
        }
    }
    sleep_us(CurrentMuxSleepTime);
}

float vlotToTempRiccnonideal(float voltage1, float voltage2) {
    const float k = 1.380649e-23f;  // Boltzmann constant in J/K
    const float q = 1.602176634e-19f; // Elementary charge in C
    const float LN10 = 2.302585093f;  // ln(10)

    float deltaV = voltage1 - voltage2;

    float T = (deltaV * q) / (nonIdealDiodeFactor * k * LN10);
    return T;  // in Kelvin
}

float voltToTempRicc(float voltage){
    float temp = 0.0;
    float breakpoint0 = 0.626882f;
    float breakpoint1 = 0.664655f;
    float breakpoint2 = 0.702428f;
    float breakpoint3 = 0.740200f;
    float breakpoint4 = 0.777973f;
    float breakpoint5 = 0.815746f;
    float breakpoint6 = 0.853519f;
    float breakpoint7 = 0.891292f;
    float breakpoint8 = 0.929064f;
    float breakpoint9 = 0.966837f;
    float breakpoint10 = 1.004610f;

    if (voltage < breakpoint1) {
        temp = -432.38f * voltage + 567.48f;
    } else if (voltage < breakpoint2) {
        temp = -524.44f * voltage + 627.91f;
    } else if (voltage < breakpoint3) {
        temp = -528.08f * voltage + 630.41f;
    } else if (voltage < breakpoint4) {
        temp = -475.00f * voltage + 591.38f;
    } else if (voltage < breakpoint5) {
        temp = -552.09f * voltage + 651.21f;
    } else if (voltage < breakpoint6) {
        temp = -538.97f * voltage + 640.64f;
    } else if (voltage < breakpoint7) {
        temp = -613.86f * voltage + 704.98f;
    } else if (voltage < breakpoint8) {
        temp = -685.73f * voltage + 767.75f;
    } else if (voltage < breakpoint9) {
        temp = -687.67f * voltage + 769.45f;
    } else {
        temp = -662.67f * voltage + 742.07f;
    }

    return temp;
}
float roundTemp(float num, int n) {
    if (num == 0.0f) return 0.0f;
    float d = ceilf(log10f(fabsf(num)));
    int power = n - (int)d;
    float magnitude = powf(10.0f, power);
    float shifted = roundf(num * magnitude);
    return shifted / magnitude;
}