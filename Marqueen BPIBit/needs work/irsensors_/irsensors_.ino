#include <SPI.h>
#include <Wire.h>

#define IR1 P13 //Left Sensor
#define IR2 P14 //Right Sensor

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SPI.end();
  //pinMode(IR1, INPUT);
  //pinMode(IR2, INPUT);
  
}

void loop() {
  Serial.print("Digital LEFT: "); Serial.print(digitalRead(IR1));
  //Serial.print(" Analog Read: "); Serial.println(analogRead(IR1));

  Serial.print(" Digital Right: "); Serial.print(digitalRead(IR2));
  //Serial.print(" Analog Read: "); Serial.println(analogRead(IR2));

Serial.println(" ");
Serial.println(" ");
  delay(500);
}
