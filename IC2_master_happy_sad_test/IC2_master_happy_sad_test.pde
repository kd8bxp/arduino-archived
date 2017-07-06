
#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

//byte x = 0;

void loop()
{
  Wire.beginTransmission(2);
 delay(5); // transmit to device #2
  Wire.send("behappy;");        // sends eight bytes
  //Wire.send(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  delay(2000);
  Wire.beginTransmission(2); // transmit to device #2
  delay(5);
  Wire.send("besad;");        // sends six bytes
  //Wire.send(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  delay(2000);

 // x++;
 // delay(500);
}
