#include "Makeblock.h"
#include <SoftwareSerial.h>
#include <Wire.h>

MeEncoderMotor motor(0x8,0x0);//Motor at Slot1

void setup() {
  motor.begin();
  Serial.begin(9600);
}
void loop() {
  motor.runSpeed(1.0);//1 round per second
  delay(2000);
  motor.runSpeed(-1.0);
  delay(2000);
}