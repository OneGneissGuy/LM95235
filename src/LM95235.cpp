#include "Arduino.h"
#include "LM95235.h"
#include <Wire.h>

/**************************************************************************/
/*
	Initializing the LM95235 class
*/
/**************************************************************************/
LM95235::LM95235(byte address)
{
    LM95235_i2cAddress = address;
}

/**************************************************************************/
/*
	Starts the I2C communication (Wire.begin) with the LM95235 unit
*/
/**************************************************************************/
void LM95235::begin(void)
{
    Wire.begin();
}

/**************************************************************************/
/*
	Parameters: None Description: Configures the operation mode of the LM95235 unit.

*/
/**************************************************************************/
void LM95235::configure()
{
    Wire.beginTransmission(LM95235_i2cAddress);
    //Point to CONFIG REGISTER LM95245_REG_RW_CONFIG1 AND UPDATE (DEFAULT on POR is 0x00)
    Wire.write(0x00); //Point to local temperature  MSB
    // Stop I2C Transmission
    Wire.endTransmission();
}

/**************************************************************************/
/*
	Reads the value data regardless of the fact it is recent or not. Works (obligatory) in continuous reading mode, optional in the single mode. Read the value int16_t instead
*/
/**************************************************************************/
int16_t LM95235::getvalue(void)
{
    int8_t highbyte;
    uint8_t lowbyte;
    Wire.requestFrom((uint8_t)LM95235_i2cAddress, (uint8_t)2);
    while (Wire.available()) // ensure all the data comes in
    {
        highbyte = Wire.read(); // high byte * B11111111
        lowbyte = Wire.read();  // low byte
    }
    value = (highbyte * 256 + lowbyte);
    return value;
}

/**************************************************************************/
/*
	Reads the unsigned temperature field
*/
/**************************************************************************/
float LM95235::get_remote_temperature()
{
    unsigned int data[2];
    point_register(LM95235_REG_R_REMOTE_TEMPH_S);
    data[0] = getuintbyte();
    point_register(LM95235_REG_R_REMOTE_TEMPL_S);
    data[1] = getuintbyte();

    float temperature = temp_from_reg_signed(data[0], data[1]) / 1000.0;
    return temperature;
}

/**************************************************************************/
/*
	Return the data in the currently active register
*/
/**************************************************************************/
void LM95235::point_register(int data_register)
{
    // Stop I2C Transmission
    Wire.beginTransmission(LM95235_i2cAddress);
    Wire.write(data_register); //Point to register address to R/W to
    // Stop I2C Transmission
    Wire.endTransmission();
}

/**************************************************************************/
/*
	Reads the unsigned temperature field
*/
/**************************************************************************/
uint8_t LM95235::getuintbyte(void)
{
    uint8_t abyte;
    Wire.requestFrom((uint8_t)LM95235_i2cAddress, (uint8_t)1);
    while (Wire.available()) // ensure all the data comes in
    {
        abyte = Wire.read(); // high byte * B11111111
    }
    value = abyte;
    return value;
}

/**************************************************************************/
/*
	Return the temperature field from unsigned temperature register values
*/
/**************************************************************************/
int LM95235::temp_from_reg_unsigned(unsigned int val_h, unsigned int val_l)
{
    return val_h * 1000 + val_l * 1000 / 256;
}

/**************************************************************************/
/*
	Return the temperature field from signed temperature register values
*/
/**************************************************************************/
int LM95235::temp_from_reg_signed(unsigned int val_h, unsigned int val_l)
{
    if (val_h & 0x80)
        return (val_h - 0x100) * 1000;
    return temp_from_reg_unsigned(val_h, val_l);
}