#include <Wire.h>
#include "src/M5StickC/M5StickC.h"

#define IOEXT_addr 0x27
#define CONFIGPORT0 0x03
#define ALLINPUT 0xFF

byte rdata = 0xFF;

void setup() {
M5.begin();
Wire.begin();

Wire.beginTransmission(IOEXT_addr);

Wire.write(CONFIGPORT0);
Wire.write(ALLINPUT);
Wire.endTransmission();

delay (50);

Wire.beginTransmission(IOEXT_addr);
Wire.write((uint8_t)0);
Wire.endTransmission();
}

void loop() {

 Serial.print("Port 1 " ); (readSensor(1));
   Serial.print("Port 2 " ); (readSensor(2));
   Serial.print("Port 3 " ); (readSensor(3));
   Serial.print("Port 4 " ); (readSensor(4));
   Serial.print("Port 5 " ); (readSensor(5));
   Serial.print("Port 6 " ); (readSensor(6));
   delay(500);

}

void readSensor(int port) {
  Wire.requestFrom(IOEXT_addr,port);

if (Wire.available()) rdata = Wire.read();

Serial.println (String(rdata, DEC));

}
