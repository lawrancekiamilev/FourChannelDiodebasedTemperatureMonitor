#include "PinDeff.h"
#include "ads1115.h"
#include "ili9341.h"
float h = 0.0;
float l = 0.0;
float HighV[4];
float LowV[4];
typedef struct
{
    float buffer[BUFFER_SIZE];
    int index;
    int count;
    float sum;
} MovingAverage;
MovingAverage channelAvgH[NUM_CHANNELS];
MovingAverage channelAvgL[NUM_CHANNELS];
// Moving average functions
void initMovingAverage(MovingAverage *ma)
{
    ma->index = 0;
    ma->count = 0;
    ma->sum = 0.0f;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        ma->buffer[i] = 0.0f;
    }
}
float addValue(MovingAverage *ma, float newValue)
{
    ma->sum -= ma->buffer[ma->index];
    ma->buffer[ma->index] = newValue;
    ma->sum += newValue;
    ma->index = (ma->index + 1) % BUFFER_SIZE;
    if (ma->count < BUFFER_SIZE)
    {
        ma->count++;
    }
    return ma->sum / ma->count;
}
int main()
{
    setup();
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        initMovingAverage(&channelAvgH[i]);
        initMovingAverage(&channelAvgL[i]);
    }

    while (true)
    {
        CheckChan();
        volatile uint64_t start_time = time_us_64();
        volatile uint64_t end_time = start_time + (500 * 1000);
        while (time_us_64() < end_time)
        {
            for (int i = 0; i < 4; i++)
            {
                float volt = 0.0f;
                if (chanConnected[i] != PreChanConnected[i])
                {
                    if (chanConnected[i])
                    {
                        PreChanConnected[i] = true;
                        readEEPROM(i);
                    }
                    else
                    {
                        PreChanConnected[i] = false;
                    }
                }
                if (chanConnected[i])
                {   
                    setChand10u(i, true);
                    volt = ReadADC(i);
                    float avgVoltH = addValue(&channelAvgH[i], volt);
                    HighV[i] = avgVoltH;
                    setChand10u(i, false);
                    volt = ReadADC(i);
                    float avgVoltL = addValue(&channelAvgL[i], volt);
                    LowV[i] = avgVoltL;
                    mutex_enter_blocking(&StopMem);
                    
                    switch (i)
                    {
                    case 0:
                        if (chan0D)
                        {
                            temp[i] = voltToTempLakeShore(avgVoltH);
                            break;
                        }
                        else
                        {
                            temp[i] = vlotToTempRiccnonideal(avgVoltH,avgVoltL);
                            break;
                        }
                    case 1:
                        if (chan1D)
                        {
                            temp[i] = voltToTempLakeShore(avgVoltH);
                            break;
                        }
                        else
                        {
                            temp[i] = vlotToTempRiccnonideal(avgVoltH,avgVoltL);
                            break;
                        }
                    case 2:
                        if (chan2D)
                        {
                            temp[i] = voltToTempLakeShore(avgVoltH);
                            break;
                        }
                        else
                        {
                            temp[i] = vlotToTempRiccnonideal(avgVoltH,avgVoltL);
                            
                            break;
                        }
                    case 3:
                        if (chan3D)
                        {
                            temp[i] = voltToTempLakeShore(avgVoltH);
                            break;
                        }
                        else
                        {
                            temp[i] = vlotToTempRiccnonideal(avgVoltH,avgVoltL);
                            break;
                        }
                    }
                    mutex_exit(&StopMem);
                }
            }

            char input = getchar_timeout_us(0);
            if (input != PICO_ERROR_TIMEOUT)
            {
                if (input == 'C' || input == 'c')
                {
                    if (chanConnected[0])
                    {
                        printf("Chan1:Name:%s:Color:%s:ID:%s:FabDate:%s;", chan0array.ArrayNAME, chan0array.ArrayColor, chan0array.ArrayID, chan0array.ArrayFabDate);
                    }
                    else
                    {
                        printf("Chan1:NC;");
                    }
                    if (chanConnected[1])
                    {
                        printf("Chan2:Name:%s:Color:%s:ID:%s:FabDate:%s;", chan1array.ArrayNAME, chan1array.ArrayColor, chan1array.ArrayID, chan1array.ArrayFabDate);
                    }
                    else
                    {
                        printf("Chan2:NC;");
                    }
                    if (chanConnected[2])
                    {
                        printf("Chan3:Name:%s:Color:%s:ID:%s:FabDate:%s;", chan2array.ArrayNAME, chan2array.ArrayColor, chan2array.ArrayID, chan2array.ArrayFabDate);
                    }
                    else
                    {
                        printf("Chan3:NC;");
                    }
                    if (chanConnected[3])
                    {
                        printf("Chan4:Name:%s:Color:%s:ID:%s:FabDate:%s;\n", chan3array.ArrayNAME, chan3array.ArrayColor, chan3array.ArrayID, chan3array.ArrayFabDate);
                    }
                    else
                    {
                        printf("Chan4:NC;\n");
                    }
                }
                else if (input == 'T' || input == 't')
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (chanConnected[i])
                        {
                            switch (i)
                            {
                            case 0:
                                printf("%s:%.2f;", chan0array.ArrayNAME, temp[i]);
                                break;
                            case 1:
                                printf("%s:%.2f;", chan1array.ArrayNAME, temp[i]);
                                break;
                            case 2:
                                printf("%s:%.2f;", chan2array.ArrayNAME, temp[i]);
                                break;
                            case 3:
                                printf("%s:%.2f", chan3array.ArrayNAME, temp[i]);
                                break;
                            }
                        }
                        else
                        {
                            switch (i)
                            {
                            case 0:
                                printf("%s:%.2f;", chan0array.ArrayNAME, 0.0);
                                break;
                            case 1:
                                printf("%s:%.2f;", chan1array.ArrayNAME, 0.0);
                                break;
                            case 2:
                                printf("%s:%.2f;", chan2array.ArrayNAME, 0.0);
                                break;
                            case 3:
                                printf("%s:%.2f", chan3array.ArrayNAME, 0.0);
                                break;
                            }
                        }
                    }
                    printf("\n");
                }
            }
        }
        watchdog_update();
    }
}

void core1_ent()
{
    init_display(ILI9341_ROTATE_90);
    drawBitmap(0, 0, cdsLogo, 320, 240);
    sleep_ms(2000);
    display_clear(ILI9341_BLACK);
    drawBitmap(270, 0, cdsLogoSmall, 50, 50);
    PrintDisplay();
    setupDiode();
    while (true)
    {
        if(b1Process){
            b1Process = false;
        }
        if (b2Process)
        {
            b2Process = false;
            volatile uint64_t start_time = time_us_64();
            volatile uint64_t end_time = start_time + (3000 * 1000);
            volatile int chan = 0;
            mutex_enter_blocking(&StopMem);
            while (time_us_64() < end_time)
            {
                if (b2Process | b1Process)
                {
                    start_time = time_us_64();
                    end_time = start_time + (3000 * 1000);
                }
                if (b1Process)
                {
                    b1Process = false;
                    chan += 1;
                    if (chan == 4)
                    {
                        chan = 0;
                    }
                }
                pickDiode(chan);
            }
            mutex_exit(&StopMem);
        }
        for (int i = 0; i < 4; i++)
        {
            if (chanConn1[i] != ChanStateChange[i])
            {
                ChanStateChange[i] = chanConn1[i];
                if (chanConn1[i])
                {
                    switch (i)
                    {
                    case 0:
                        PrintArrayName(1, chan0array.ArrayNAME);
                        break;
                    case 1:
                        PrintArrayName(60, chan1array.ArrayNAME);
                        break;
                    case 2:
                        PrintArrayName(120, chan2array.ArrayNAME);
                        break;
                    case 3:
                        PrintArrayName(180, chan3array.ArrayNAME);
                        break;
                    }
                }
                else
                {
                    switch (i)
                    {
                    case 0:
                        ILI9341_fillRect(70, 1, 200, 15, ILI9341_BLACK);
                        ILI9341_fillRect(1, 20, 270, 34, ILI9341_BLACK);
                        drawString(75, 1, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);
                        if (chan0D)
                        {
                            drawString(200, 20, "L", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 20, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        else
                        {
                            drawString(200, 20, "R", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 20, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        break;
                    case 1:
                        ILI9341_fillRect(70, 60, 200, 15, ILI9341_BLACK);
                        ILI9341_fillRect(1, 80, 270, 34, ILI9341_BLACK);
                        drawString(75, 60, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);
                        if (chan1D)
                        {
                            drawString(200, 80, "L", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 80, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        else
                        {
                            drawString(200, 80, "R", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 80, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        break;
                    case 2:
                        ILI9341_fillRect(70, 120, 270, 15, ILI9341_BLACK);
                        ILI9341_fillRect(1, 140, 270, 34, ILI9341_BLACK);
                        drawString(75, 120, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);
                        if (chan2D)
                        {
                            drawString(200, 140, "L", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 140, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        else
                        {
                            drawString(200, 140, "R", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 140, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        break;
                    case 3:
                        ILI9341_fillRect(70, 180, 270, 15, ILI9341_BLACK);
                        ILI9341_fillRect(1, 200, 270, 34, ILI9341_BLACK);
                        drawString(75, 180, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);
                        if (chan3D)
                        {
                            drawString(200, 200, "L", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 200, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        else
                        {
                            drawString(200, 200, "R", ILI9341_BLACK, ILI9341_WHITE, 3);
                            sleep_ms(50);
                            drawString(200, 200, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
                        }
                        break;
                    }
                }
            }
            else
            {
                if (chanConn1[i])
                {
                    switch (i) {
                        case 0:
                            PrintTemp(1, 20, temp[i], HighV[i], LowV[i]);
                            break;
                        case 1:
                            PrintTemp(1, 80, temp[i], HighV[i], LowV[i]);
                            break;
                        case 2:
                            PrintTemp(1, 140, temp[i], HighV[i], LowV[i]);
                            break;
                        case 3:
                            PrintTemp(1, 200, temp[i], HighV[i], LowV[i]);
                            break;
                    }

                }
            }
        }
    }
}