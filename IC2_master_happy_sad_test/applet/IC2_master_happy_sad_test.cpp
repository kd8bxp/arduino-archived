// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

#include <Wire.h>

#include "WProgram.h"
void setup();
void loop();
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(2); // transmit to device #2
  Wire.send("behappy");        // sends seven bytes
  //Wire.send(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  delay(2000);
  Wire.beginTransmission(2); // transmit to device #2
  Wire.send("besad");        // sends five bytes
  //Wire.send(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  delay(2000);

 // x++;
 // delay(500);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

