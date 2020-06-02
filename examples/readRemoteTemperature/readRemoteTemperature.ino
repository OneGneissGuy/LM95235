#include <Arduino.h>
#include <Wire.h>
#include <LM95235.h>
// Example code to use the LM95235 Precision Remote Diode Temperature Sensor
// Reads the tempertuer of a remote 3904 NPN transistor configured as a diode

// Instantiate LM95235 object
#define LM95235_addr 0x18 //A0 tied to ground

LM95235 lmtemp = LM95235(LM95235_addr);
void setup()
{
    // Initialize Serial Communication, set baud rate = 9600
    Serial.begin(9600);
    // Initialise I2C communication as MASTER
    lmtemp.begin();
    lmtemp.configure()
        delay(300);
}

void loop()
{
    float temp = lmtemp.get_remote_temperature();
    Serial.print("Remote Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    delay(500);
}