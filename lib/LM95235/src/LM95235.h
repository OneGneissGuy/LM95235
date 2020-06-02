/**************************************************************************/
/*

    The LM95235 is an 11-bit digital temperature sensor
    with a 2-wire System Management Bus (SMBus)
    interface and TruTherm technology that can monitor
    the temperature of a remote diode as well as its own
    temperature.The LM95235 can be used to very
    accurately monitor the temperature of external
    devices  or a diode-connected MMBT3904 transistor.

	
	The unit is made by Texas Instruments, 
    Precision Remote Diode Temperature Sensor with SMBus Interface and TruThermTM
    Technology
	https://www.ti.com/product/LM95235?qgpn=lm95235
	
	The library updated on XXXXXX by JohnFranco Saraceno
    Github: https://github.com/onegneissguy/LM95235
	
	GNU General Public License v3.0
	Distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************
	I2C ADDRESS/BITS
**************************************************************************/
#define lm95235_DEFAULT_ADDRESS (0x18) //  A0->GND                                                                  \
                                       // #define lm95235_DEFAULT_ADDRESS                       (0x29)  //  A0->VDD \
                                       // #define lm95235_DEFAULT_ADDRESS                       (0x4C)  //  A0->VDD/2

/**************************************************************************
	CONFIGURATION REGISTER
**************************************************************************/
/* LM95235 registers */
/* general registers */
#define LM95235_REG_RW_CONFIG1 (0x03)
#define LM95235_REG_RW_CONVERS_RATE (0x04)
#define LM95235_REG_W_ONE_SHOT (0x0F)

/* diode configuration */
#define LM95235_REG_RW_CONFIG2 (0xBF)
#define LM95235_REG_RW_REMOTE_OFFH (0x11)
#define LM95235_REG_RW_REMOTE_OFFL (0x12)

/* status registers */
#define LM95235_REG_R_STATUS1 (0x02)
#define LM95235_REG_R_STATUS2 (0x33)

/* limit registers */
#define LM95235_REG_RW_REMOTE_OS_LIMIT (0x07)
#define LM95235_REG_RW_LOCAL_OS_TCRIT_LIMIT (0x20)
#define LM95235_REG_RW_REMOTE_TCRIT_LIMIT (0x19)
#define LM95235_REG_RW_COMMON_HYSTERESIS (0x21)

/* temperature signed */
#define LM95235_REG_R_LOCAL_TEMPH_S (0x00)
#define LM95235_REG_R_LOCAL_TEMPL_S (0x30)
#define LM95235_REG_R_REMOTE_TEMPH_S (0x01)
#define LM95235_REG_R_REMOTE_TEMPL_S (0x10)
/* temperature unsigned */
#define LM95235_REG_R_REMOTE_TEMPH_U (0x31)
#define LM95235_REG_R_REMOTE_TEMPL_U (0x32)

/* id registers */
#define LM95235_REG_R_MAN_ID (0xFE)
#define LM95235_REG_R_CHIP_ID (0xFF)

/* LM95235 specific bitfields */
#define CFG_STOP (0x40)
#define CFG_REMOTE_TCRIT_MASK (0x10)
#define CFG_REMOTE_OS_MASK (0x08)
#define CFG_LOCAL_TCRIT_MASK (0x04)
#define CFG_LOCAL_OS_MASK (0x02)

#define CFG2_OS_A0 (0x40)
#define CFG2_DIODE_FAULT_OS (0x20)
#define CFG2_DIODE_FAULT_TCRIT (0x10)
#define CFG2_REMOTE_TT (0x08)
#define CFG2_REMOTE_FILTER_DIS (0x00)
#define CFG2_REMOTE_FILTER_EN (0x06)

/* conversation rate in ms */
#define RATE_CR0063 (0x00)
#define RATE_CR0364 (0x01)
#define RATE_CR1000 (0x02)
#define RATE_CR2500 (0x03)

#define STATUS1_ROS (0x10)
#define STATUS1_DIODE_FAULT (0x04)
#define STATUS1_RTCRIT (0x02)
#define STATUS1_LOC (0x01)

#define MANUFACTURER_ID (0x01)
#define LM95235_REVISION (0xB1)
#define LM95245_REVISION (0xB3)

class LM95235
{
public:
    LM95235(uint8_t address);
    int16_t value; // value of the ADC returned
    void begin(void);
    int16_t getvalue(void);    // reading the value regardless of the fact it is available
    uint8_t getuintbyte(void); // reading an uint8_t value
    float get_remote_temperature();
    void point_register(int reg_addr);

    // void configure(uint8_t singleMode, uint8_t dataRate, uint8_t gain);
    void configure(void);
    uint8_t conversionDone();
    void startSingleConversion();
    int temp_from_reg_unsigned(unsigned int val_h, unsigned int val_l);
    int temp_from_reg_signed(unsigned int val_h, unsigned int val_l);

private:
    uint8_t LM95235_i2cAddress;          // I2C address of the unit
    uint8_t currentRegister = B00001100; // the last register used
};
