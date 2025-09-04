#include "PinDeff.h"
#include "ili9341.h"
#include <stdlib.h>
#include <string.h>

uint16_t width = 320;
uint16_t height = 240;
uint8_t display_rotation = 0;

static inline void select_display(void) {
    gpio_put(PIN_CS, 0);
}

static inline void deselect_display(void) {
    gpio_put(PIN_CS, 1);
}

void write_cmd(uint8_t command) {
    gpio_put(PIN_DC, 0);
    select_display();
    spi_write_blocking(SPI_PORT, &command, 1);
    deselect_display();
}

void write_data(const uint8_t *data, size_t len) {
    if (!data || len == 0) return;
    gpio_put(PIN_DC, 1);
    select_display();
    spi_write_blocking(SPI_PORT, data, len);
    deselect_display();
}

void display_reset() {
    gpio_put(PIN_RST, 0);
    sleep_ms(50);
    gpio_put(PIN_RST, 1);
    sleep_ms(50);
}

void block(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *data, size_t len) {
    if (!data || len == 0) return;
    uint8_t col_data[] = { x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF };
    uint8_t row_data[] = { y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF };

    write_cmd(ILI9341_SET_COLUMN);
    write_data(col_data, sizeof(col_data));

    write_cmd(ILI9341_SET_PAGE);
    write_data(row_data, sizeof(row_data));

    write_cmd(ILI9341_WRITE_RAM);
    write_data(data, len);
}

void display_clear(uint16_t color) {
    int line_width = width * 2;
    uint8_t *line = malloc(line_width);
    if (!line) return;

    uint8_t hi = color >> 8, lo = color & 0xFF;
    for (int i = 0; i < line_width; i += 2) {
        line[i] = hi;
        line[i + 1] = lo;
    }

    for (int y = 0; y < height; y++) {
        block(0, y, width - 1, y, line, line_width);
    }
    free(line);
}

void init_display(uint8_t rotation) {
    display_rotation = rotation;
    gpio_init(PIN_CS); gpio_set_dir(PIN_CS, GPIO_OUT); gpio_put(PIN_CS, 1);
    gpio_init(PIN_DC); gpio_set_dir(PIN_DC, GPIO_OUT); gpio_put(PIN_DC, 0);
    gpio_init(PIN_RST); gpio_set_dir(PIN_RST, GPIO_OUT); gpio_put(PIN_RST, 1);

    setRotation();
    display_reset();
    spi_init(SPI_PORT, 1000000000);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);

    display_reset();

    static const uint8_t pwctrb_data[]  = {0x00, 0xC1, 0x30};
    static const uint8_t posc_data[]    = {0x64, 0x03, 0x12, 0x81};
    static const uint8_t dtca_data[]    = {0x85, 0x00, 0x78};
    static const uint8_t pwctra_data[]  = {0x39, 0x2C, 0x00, 0x34, 0x02};
    static const uint8_t pumprc_data[]  = {0x20};
    static const uint8_t dtcb_data[]    = {0x00, 0x00};
    static const uint8_t pwctr1_data[]  = {0x23};
    static const uint8_t pwctr2_data[]  = {0x10};
    static const uint8_t vmctr1_data[]  = {0x3E, 0x28};
    static const uint8_t vmctr2_data[]  = {0x86};
    static uint8_t madctl_data[]        = {0};
    static const uint8_t vscrsadd_data[]= {0x00};
    static const uint8_t pixfmt_data[]  = {0x55};
    static const uint8_t frmctr1_data[] = {0x00, 0x18};
    static const uint8_t dfunctr_data[] = {0x08, 0x82, 0x27};
    static const uint8_t enable3g_data[]= {0x00};
    static const uint8_t gammaset_data[]= {0x01};
    static const uint8_t gmctrp1_data[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    static const uint8_t gmctrn1_data[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};

    madctl_data[0] = rotation;

    static const struct { uint8_t cmd; const uint8_t *data; uint8_t len; } init_seq[] = {
        { ILI9341_SWRESET, NULL, 0 },
        { ILI9341_PWCTRB, pwctrb_data, sizeof(pwctrb_data) },
        { ILI9341_POSC, posc_data, sizeof(posc_data) },
        { ILI9341_DTCA, dtca_data, sizeof(dtca_data) },
        { ILI9341_PWCTRA, pwctra_data, sizeof(pwctra_data) },
        { ILI9341_PUMPRC, pumprc_data, sizeof(pumprc_data) },
        { ILI9341_DTCB, dtcb_data, sizeof(dtcb_data) },
        { ILI9341_PWCTR1, pwctr1_data, sizeof(pwctr1_data) },
        { ILI9341_PWCTR2, pwctr2_data, sizeof(pwctr2_data) },
        { ILI9341_VMCTR1, vmctr1_data, sizeof(vmctr1_data) },
        { ILI9341_VMCTR2, vmctr2_data, sizeof(vmctr2_data) },
        { ILI9341_MADCTL, madctl_data, sizeof(madctl_data) },
        { ILI9341_VSCRSADD, vscrsadd_data, sizeof(vscrsadd_data) },
        { ILI9341_PIXFMT, pixfmt_data, sizeof(pixfmt_data) },
        { ILI9341_FRMCTR1, frmctr1_data, sizeof(frmctr1_data) },
        { ILI9341_DFUNCTR, dfunctr_data, sizeof(dfunctr_data) },
        { ILI9341_ENABLE3G, enable3g_data, sizeof(enable3g_data) },
        { ILI9341_GAMMASET, gammaset_data, sizeof(gammaset_data) },
        { ILI9341_GMCTRP1, gmctrp1_data, sizeof(gmctrp1_data) },
        { ILI9341_GMCTRN1, gmctrn1_data, sizeof(gmctrn1_data) },
        { ILI9341_SLPOUT, NULL, 0 },
        { ILI9341_DISPLAY_ON, NULL, 0 }
    };

    for (size_t i = 0; i < sizeof(init_seq) / sizeof(init_seq[0]); i++) {
        write_cmd(init_seq[i].cmd);
        if (init_seq[i].data && init_seq[i].len)
            write_data(init_seq[i].data, init_seq[i].len);
        sleep_ms(5);
    }

    sleep_ms(100);
    display_clear(0);
}

void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h) {
    if (!bitmap || w <= 0 || h <= 0) return;

    int byteWidth = (w + 7) / 8;
    int buffer_size = w * h * 2;
    uint8_t *buffer = malloc(buffer_size);
    if (!buffer) return;

    int buf_idx = 0;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            if (bitmap[j * byteWidth + i / 8] & (128 >> (i & 7))) {
                buffer[buf_idx++] = (ILI9341_RED >> 8) & 0xFF;
                buffer[buf_idx++] = ILI9341_RED & 0xFF;
            } else {
                buffer[buf_idx++] = (ILI9341_BLACK >> 8) & 0xFF;
                buffer[buf_idx++] = ILI9341_BLACK & 0xFF;
            }
        }
    }

    ILI9341_setAddrWindow(x, y, x + w - 1, y + h - 1);
    write_data(buffer, buffer_size);
    free(buffer);
}
void ILI9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    write_cmd(ILI9341_SET_COLUMN); // Column address set
    uint8_t col_data[] = {(x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF};
    write_data(col_data, sizeof(col_data)); // Send XSTART and XEND

    write_cmd(ILI9341_SET_PAGE); // Row address set
    uint8_t page_data[] = {(y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF};
    write_data(page_data, sizeof(page_data)); // Send YSTART and YEND

    write_cmd(ILI9341_WRITE_RAM); // Write to RAM
}

void ILI9341_drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) return;

    ILI9341_setAddrWindow(x, y, x + 1, y + 1);
    uint8_t color_data[] = { color >> 8, color & 0xFF };
    write_data(color_data, sizeof(color_data));
}

void setRotation()
{
    if (display_rotation == ILI9341_ROTATE_90 || display_rotation == ILI9341_ROTATE_270)
    {
        width = 320;
        height = 240;
    }
    else
    {
        width = 240;
        height = 320;
    };
}

uint16_t interpolateColor(float value)
{
    // Ensure value is within the expected range
    if (value < 70)
        value = 70;
    if (value > 298)
        value = 298;

    // Normalize the value between 0.0 and 1.0
    float normalized = (value - 70) / (298 - 70);

    // Interpolate red and blue components (no green component)
    uint8_t red = (uint8_t)(255 * normalized);          // Red increases with value
    uint8_t blue = (uint8_t)(255 * (1.0 - normalized)); // Blue decreases with value

    // Convert to RGB565 format
    uint16_t color = ((red & 0xF8) << 8) | ((blue & 0xF8) >> 3);

    return color;
}

void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
    if ((x >= width) || (y >= height) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0)) return;

    for (int8_t i = 0; i < 6; i++) {
        uint8_t line = (i == 5) ? 0x0 : font[c * 5 + i];
        for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 0x1) {
                if (size == 1)
                    ILI9341_drawPixel(x + i, y + j, color);
                else
                    ILI9341_fillRect(x + (i * size), y + (j * size), size, size, color);
            } else if (bg != color) {
                if (size == 1)
                    ILI9341_drawPixel(x + i, y + j, bg);
                else
                    ILI9341_fillRect(x + i * size, y + j * size, size, size, bg);
            }
        }
    }
}

void drawString(int16_t x, int16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size) {
    if (!str) return;
    while (*str) {
        drawChar(x, y, *str, color, bg, size);
        x += 6 * size;
        if (x + 6 * size >= width) {
            x = 0;
            y += 8 * size;
        }
        str++;
    }
}
void ILI9341_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    if (h < 0)
    { // Handle negative height by adjusting y and h
        h = -h;
        y -= h;
    }

    // Boundary check to avoid drawing out of bounds
    if (x < 0 || x >= width || y >= height || (y + h - 1) < 0)
    {
        return;
    }

    // Clip the top of the line if y < 0
    if (y < 0)
    {
        h += y;
        y = 0;
    }

    // Clip the bottom of the line
    if ((y + h) > h)
    {
        h = h - y;
    }

    // Set the address window for the vertical line
    ILI9341_setAddrWindow(x, y, x, y + h - 1);

    // Prepare the color data for each pixel
    uint8_t color_data[] = {(color >> 8) & 0xFF, color & 0xFF};

    // Write color data to each pixel in the vertical line
    for (int16_t i = 0; i < h; i++)
    {
        write_data(color_data, sizeof(color_data));
    }
}

void ILI9341_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (w <= 0 || h <= 0) return;

    if (x >= width || y >= height || (x + w - 1) < 0 || (y + h - 1) < 0) return;

    if ((x + w) > width) w = width - x;
    if ((y + h) > height) h = height - y;

    ILI9341_setAddrWindow(x, y, x + w - 1, y + h - 1);

    uint8_t color_data[] = { color >> 8, color & 0xFF };
    int pixels = w * h;
    for (int i = 0; i < pixels; i++) {
        write_data(color_data, sizeof(color_data));
    }
}
