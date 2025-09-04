#ifndef ads1115_H
#define ads1115_H


#define ADS1115_I2C_ADDR                0x48
/* i2c_write does not work if I pass the register address as a #define. It only works if the address is a const uint8_t and it is passed as a pointer (&)*/
#define ADS1115_POINTER_CONVERSION      0x00
#define ADS1115_POINTER_CONFIGURATION   0x01
#define ADS1115_POINTER_LO_THRESH       0x02
#define ADS1115_POINTER_HI_THRESH       0x03
#define ADS1115_REG_CONFIG_CQUE_1CONV   0x0000

// Operational status; bit 15
#define ADS1115_STATUS_MASK             0x8000
#define ADS1115_STATUS_START            0x01
#define ADS1115_STATUS_BUSY             0x00
#define ADS1115_REG_CONFIG_OS_SINGLE    0x8000

// Input multiplexer configuration; bits 14-12
#define ADS1115_MUX_MASK                0x7000
#define ADS1115_MUX_DIFF_0_1            0x0000
#define ADS1115_MUX_DIFF_0_3            0x1000
#define ADS1115_MUX_DIFF_1_3            0x2000
#define ADS1115_MUX_DIFF_2_3            0x3000
#define ADS1115_MUX_SINGLE_0            0x4000
#define ADS1115_MUX_SINGLE_1            0x5000
#define ADS1115_MUX_SINGLE_2            0x6000
#define ADS1115_MUX_SINGLE_3            0x7000
// Programmable gain amplifier; bits 11-9
// Full-scale range (FSR) options are ±6.144V, ±4.096V, ±2.048V,
// ±1.024V, ±0.512V, ±0.256V.
#define ADS1115_PGA_MASK                0x0E00
#define ADS1115_PGA_6_144               0x0000
#define ADS1115_PGA_4_096               0x0200
#define ADS1115_PGA_2_048               0x0400 // default
#define ADS1115_PGA_1_024               0x0600
#define ADS1115_PGA_0_512               0x0800
#define ADS1115_PGA_0_256               0x0A00

// Operating mode; bit 8
#define ADS1115_MODE_MASK               0x0100
#define ADS1115_MODE_CONTINUOUS         0x0000
#define ADS1115_MODE_SINGLE_SHOT        0x0100 // default

// Data rate; configuration register bits 7-5.
#define ADS1115_RATE_MASK               0x00E0
#define ADS1115_RATE_8_SPS              0x0000
#define ADS1115_RATE_16_SPS             0x0020
#define ADS1115_RATE_32_SPS             0x0040
#define ADS1115_RATE_64_SPS             0x0060
#define ADS1115_RATE_128_SPS            0x0080 ///< default
#define ADS1115_RATE_250_SPS            0x00A0
#define ADS1115_RATE_475_SPS            0x00C0
#define ADS1115_RATE_860_SPS            0x00E0

// Comparator mode, bit 4
#define ADS1115_COMP_MODE_MASK          0x0010
#define ADS1115_COMPARATOR_TRADITIONAL  0x0000 // default
#define ADS1115_COMPARATOR_WINDOW       0x0010
#define ADS1X15_REG_CONFIG_CPOL_ACTVLOW 0x0000
// Comparator polarity, bit 3
#define ADS1115_COMP_POL_MASK           0x0008
#define ADS1115_COMPARATOR_POLARITY_LO  0x0000 // default
#define ADS1115_COMPARATOR_POLARITY_HI  0x0008

// Latching comparator, bit 2
#define ADS1115_COMP_LAT_MASK           0x0004
#define ADS1115_COMPARATOR_NONLATCHING  0x0000 // default
#define ADS1115_COMPARATOR_LATCHING     0x0004

// Comparator queue and disable, bits 1-0
#define ADS1115_COMP_QUE_MASK           0x0003
#define ADS1115_COMPARATOR_QUE_1        0x0000
#define ADS1115_COMPARATOR_QUE_2        0x0001
#define ADS1115_COMPARATOR_QUE_4        0x0002
#define ADS1115_COMPARATOR_QUE_DISABLE  0x0003 // default



void writeReg(uint8_t reg, uint16_t value);
void setAdc(int mux);
float ads1115_raw_to_volts(int16_t counts);
float ReadADC(int chan);
int16_t getConversion();
uint16_t readReg(uint8_t reg);
#endif