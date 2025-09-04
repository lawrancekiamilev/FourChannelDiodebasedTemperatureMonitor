#ifndef ILI9341_H
#define ILI9341_H

// Command constants from ILI9341 datasheet
#define ILI9341_NOP                   0x00  // No-op
#define ILI9341_SWRESET               0x01  // Software reset
#define ILI9341_RDDID                 0x04  // Read display ID info
#define ILI9341_RDDST                 0x09  // Read display status
#define ILI9341_SLPIN                 0x10  // Enter sleep mode
#define ILI9341_SLPOUT                0x11  // Exit sleep mode
#define ILI9341_PTLON                 0x12  // Partial mode on
#define ILI9341_NORON                 0x13  // Normal display mode on
#define ILI9341_RDMODE                0x0A  // Read display power mode
#define ILI9341_RDMADCTL              0x0B  // Read display MADCTL
#define ILI9341_RDPIXFMT              0x0C  // Read display pixel format
#define ILI9341_RDIMGFMT              0x0D  // Read display image format
#define ILI9341_RDSELFDIAG            0x0F  // Read display self-diagnostic
#define ILI9341_INVOFF                0x20  // Display inversion off
#define ILI9341_INVON                 0x21  // Display inversion on
#define ILI9341_GAMMASET              0x26  // Gamma set
#define ILI9341_DISPLAY_OFF           0x28  // Display off
#define ILI9341_DISPLAY_ON            0x29  // Display on
#define ILI9341_SET_COLUMN            0x2A  // Column address set
#define ILI9341_SET_PAGE              0x2B  // Page address set
#define ILI9341_WRITE_RAM             0x2C  // Memory write
#define ILI9341_READ_RAM              0x2E  // Memory read
#define ILI9341_PTLAR                 0x30  // Partial area
#define ILI9341_VSCRDEF               0x33  // Vertical scrolling definition
#define ILI9341_MADCTL                0x36  // Memory access control
#define ILI9341_VSCRSADD              0x37  // Vertical scrolling start address
#define ILI9341_PIXFMT                0x3A  // COLMOD: Pixel format set
#define ILI9341_WRITE_DISPLAY_BRIGHTNESS 0x51  // Brightness control
#define ILI9341_READ_DISPLAY_BRIGHTNESS  0x52
#define ILI9341_WRITE_CTRL_DISPLAY    0x53
#define ILI9341_READ_CTRL_DISPLAY     0x54
#define ILI9341_WRITE_CABC            0x55  // Write Content Adaptive Brightness Control
#define ILI9341_READ_CABC             0x56  // Read Content Adaptive Brightness Control
#define ILI9341_WRITE_CABC_MINIMUM    0x5E  // Write CABC Minimum Brightness
#define ILI9341_READ_CABC_MINIMUM     0x5F  // Read CABC Minimum Brightness
#define ILI9341_FRMCTR1               0xB1  // Frame rate control (In normal mode/full colors)
#define ILI9341_FRMCTR2               0xB2  // Frame rate control (In idle mode/8 colors)
#define ILI9341_FRMCTR3               0xB3  // Frame rate control (In partial mode/full colors)
#define ILI9341_INVCTR                0xB4  // Display inversion control
#define ILI9341_DFUNCTR               0xB6  // Display function control
#define ILI9341_PWCTR1                0xC0  // Power control 1
#define ILI9341_PWCTR2                0xC1  // Power control 2
#define ILI9341_PWCTRA                0xCB  // Power control A
#define ILI9341_PWCTRB                0xCF  // Power control B
#define ILI9341_VMCTR1                0xC5  // VCOM control 1
#define ILI9341_VMCTR2                0xC7  // VCOM control 2
#define ILI9341_RDID1                 0xDA  // Read ID 1
#define ILI9341_RDID2                 0xDB  // Read ID 2
#define ILI9341_RDID3                 0xDC  // Read ID 3
#define ILI9341_RDID4                 0xDD  // Read ID 4
#define ILI9341_GMCTRP1               0xE0  // Positive gamma correction
#define ILI9341_GMCTRN1               0xE1  // Negative gamma correction
#define ILI9341_DTCA                  0xE8  // Driver timing control A
#define ILI9341_DTCB                  0xEA  // Driver timing control B
#define ILI9341_POSC                  0xED  // Power on sequence control
#define ILI9341_ENABLE3G              0xF2  // Enable 3 gamma control
#define ILI9341_PUMPRC                0xF7  // Pump ratio control

#define ILI9341_WHITE 0xFFFF
#define ILI9341_BLACK 0x0000
// Rotation values (MADCTL memory access control)
#define ILI9341_ROTATE_0              0x88
#define ILI9341_ROTATE_90             0xE8
#define ILI9341_ROTATE_180            0x48
#define ILI9341_ROTATE_270            0x28
#define ILI9341_RED                   0xF800
extern uint8_t cdsLogo[];
extern uint8_t cdsLogoSmall[];
extern uint16_t width;
extern uint16_t height;
extern uint8_t display_rotation;
extern const unsigned char font[];
// Function declarations

void write_cmd(uint8_t command);
void write_data(const uint8_t *data, size_t len);
void display_reset();
void block(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *data, size_t len);
void display_clear(uint16_t color);
void init_display(uint8_t rotation);
void setRotation();
void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h);
void ILI9341_drawPixel(int16_t x, int16_t y, uint16_t color);
void ILI9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void drawString(int16_t x, int16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
uint16_t interpolateColor(float value);
void ILI9341_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ILI9341_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void PrintArrayName(int y,char *arr);
#endif 
