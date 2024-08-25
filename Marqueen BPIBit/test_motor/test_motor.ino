
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
Wire.begin();
delay(1000);
forward();
delay(5000);
stop();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void forward() {
  int _m1spd = 50;
  int _m2spd = 50;
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write(_m1spd); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write(_m2spd); //speed
  Wire.endTransmission();
}

void stop() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
//_m1spd = 0;
//_m2spd = 0;
}
