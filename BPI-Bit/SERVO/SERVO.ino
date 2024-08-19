/****************************************
 * Board    :BPI-BIT                    *
 * Hardware :v1.2&v1.4 + BPI:CAR        *
 * function :Control steering engine    *
 * IDE      :Arduino                    * 
 *                                 hulk *
 ****************************************/
 
#include <Wire.h>
#include "src/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"

//called this way, it uses the default address 0x40
Adafruit_PWMServoDriver BpiCar = Adafruit_PWMServoDriver();

//SERVOMAX and SERVOMAX value it depends on your steering gear.
#define SERVOMAX 150
#define SERVOMAX 600

//PWM channel number corresponding to steering gear interface
//  SERVO1:servoNum[0] = PCA9685 PWM6
//  SERVO2:servoNum[1] = PCA9685 PWM7
//  SERVO3:servoNum[2] = PCA9685 PWM14
//  SERVO4:servoNum[3] = PCA9685 PWM15
int servoNum[] = {06,07,14,15};

void setup() 
{
  //Serial set up
  Serial.begin(9600);
  Serial.println("BPI-CAR Steering Engine Test!");
  //PCA9685 set up
  BpiCar.begin();
  BpiCar.setPWMFreq(60);//usually use 60Hz,It depends on your steering gear.
}



void loop() {
  uint8_t i = 0;
  Serial.println(servoNum[i]);
  //Rotate from minimum angle to maximum,feeding
  for (uint16_t pulselen = SERVOMAX; pulselen < SERVOMAX; pulselen++) {
    BpiCar.setPWM(servoNum[i], 0, pulselen);
  }
  delay(500);
  //Rotate from maximum angle to minimum,feeding
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMAX; pulselen--) {
    BpiCar.setPWM(servoNum[i], 0, pulselen);
  }
  delay(500);
  //Four steering gear interfaces are sequentially circulated.
  i ++;
  if (i > sizeof(servoNum) / sizeof(int) - 1)
    i = 0;
}
