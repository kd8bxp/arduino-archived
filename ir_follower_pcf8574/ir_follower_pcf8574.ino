
#include <Wire.h>    // Required for I2C communication
#include "src/PCF8574/PCF8574.h" // https://github.com/xreef/PCF8574_library
#include "src/M5Atom/M5Atom.h"

PCF8574 pcf8574(&Wire1, 0x20);

/** setup() */
void setup() {
Wire1.begin(26,32);
M5.begin(true,true,true);

  /* Setup serial for debug */
  Serial.begin(115200);
  
  /* Start I2C bus and PCF8574 instance */
  pcf8574.begin();
  
  /* Setup some PCF8574 pins for demo */
  pcf8574.pinMode(0, INPUT);
  pcf8574.pinMode(1, INPUT);
  pcf8574.pinMode(2, INPUT);
  pcf8574.pinMode(3, INPUT);
  pcf8574.pinMode(4, INPUT);
  pcf8574.pinMode(5, INPUT);
  pcf8574.pinMode(6, INPUT);
  pcf8574.pinMode(7, INPUT);
  
 
}


void loop() {
   //PCF8574::DigitalInput di = pcf8574.digitalReadAll();
   Serial.print("Port 1 " ); Serial.println(pcf8574.digitalRead(2));
   Serial.print("Port 2 " ); Serial.println(pcf8574.digitalRead(3));
   Serial.print("Port 3 " ); Serial.println(pcf8574.digitalRead(4));
   Serial.print("Port 4 " ); Serial.println(pcf8574.digitalRead(5));
   Serial.print("Port 5 " ); Serial.println(pcf8574.digitalRead(6));
   Serial.print("Port 6 " ); Serial.println(pcf8574.digitalRead(7));
   
   delay(500);
}
