#include "PinDeff.h"
#include "ili9341.h"

void PrintDisplay()
{
    ILI9341_fillRect(1, 1, 270, 15, ILI9341_BLACK);
    ILI9341_fillRect(1, 60, 270, 15, ILI9341_BLACK);
    ILI9341_fillRect(1, 120, 270, 15, ILI9341_BLACK);
    ILI9341_fillRect(1, 180, 270, 15, ILI9341_BLACK);
    drawString(1, 1, "Chan1:", ILI9341_WHITE, ILI9341_BLACK, 2);
    drawString(75, 1, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);

    drawString(1, 60, "Chan2:", ILI9341_WHITE, ILI9341_BLACK, 2);
    drawString(75, 60, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);

    drawString(1, 120, "Chan3:", ILI9341_WHITE, ILI9341_BLACK, 2);
    drawString(75, 120, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);

    drawString(1, 180, "Chan4:", ILI9341_WHITE, ILI9341_BLACK, 2);
    drawString(75, 180, "Disconnected", ILI9341_WHITE, ILI9341_BLACK, 2);
}
void PrintArrayName(int y, char *arr)
{
    ILI9341_fillRect(70, y, 200, 15, ILI9341_BLACK);
    sleep_ms(200);
    drawString(75, y, arr, ILI9341_WHITE, ILI9341_BLACK, 2);
}
void PrintTemp(int x,int y, float temp,float Vh, float Vl)
{   
    char buffer1[17];
    char buffer2[17];
    char buffer[17];
    uint16_t color = interpolateColor(temp);
    sprintf(buffer, "%.2fK ", temp);
    drawString(x, y, buffer, color, ILI9341_BLACK, 3);
    sprintf(buffer1, "%.6fV(10u) ", Vh);
    drawString(x, y+25, buffer1, ILI9341_WHITE, ILI9341_BLACK, 1);
    sprintf(buffer2, "%.6fV(1u) ", Vl);
    drawString(100, y+25, buffer2, ILI9341_WHITE, ILI9341_BLACK, 1);
}
void pickDiode(int chan)
{
    switch (chan)
    {
    case 0:
        if (b2Process)
        {
            b2Process = false;
            chan0D = !chan0D;
        }
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
        if (b2Process)
        {
            b2Process = false;
            chan1D = !chan1D;
        }
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
        if (b2Process)
        {
            b2Process = false;
            chan2D = !chan2D;
        }
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
        if (b2Process)
        {
            b2Process = false;
            chan3D = !chan3D;
        }
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
void setupDiode()
{

    if (chan3D)
    {
        drawString(200, 20, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
    }
    else
    {
        drawString(200, 20, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
    }

    if (chan3D)
    {
        drawString(200, 80, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
    }
    else
    {
        drawString(200, 80, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
    }

    if (chan3D)
    {
        drawString(200, 140, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
    }
    else
    {
        drawString(200, 140, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
    }

    if (chan3D)
    {
        drawString(200, 200, "L", ILI9341_WHITE, ILI9341_BLACK, 3);
    }
    else
    {
        drawString(200, 200, "R", ILI9341_WHITE, ILI9341_BLACK, 3);
    }
}