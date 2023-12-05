#include "I2C_slave.h"
#include <Arduino.h>

uint8_t tcs_status = 0;
uint8_t ultra_dis = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Start...");
    

  // wrSensorReg8_8(STM32_1_SLAVE_ADDR, AVOID_REG, 1); // avoid mode, works, but only at full speed, and only with ir sensors, doesn't let you read the ir senors :-(
   
}

void loop() {
 /* 
  *  Motor Function seem to work, but only after removing the color sensor on the bottom of the robot
  *  wrSensorReg8_8(STM32_1_SLAVE_ADDR, SPEED_REG, 25);
  wrSensorReg8_8(STM32_1_SLAVE_ADDR, MOTOR_REG, 6);
  delay(500);
  wrSensorReg8_8(STM32_1_SLAVE_ADDR, MOTOR_REG, 1);
  delay(500);
  wrSensorReg8_8(STM32_2_SLAVE_ADDR, SONG_REG, 4);
*/
   rdSensorReg8_8(STM32_2_SLAVE_ADDR, ULTRA_REG, &ultra_dis);
  Serial.println(ultra_dis);
  delay(250);
 // Wire.beginTransmission(32);
 //   Wire.write(233);
 //   Wire.write(3);
 //   Wire.endTransmission();
  //while(1);
//wrSensorReg8_8(STM32_2_SLAVE_ADDR, LED_SET_REG, 0);
//delay(1);
// wrSensorReg8_8(STM32_2_SLAVE_ADDR, LED_G_REG, 255);
// delay(1);
// wrSensorReg8_8(STM32_2_SLAVE_ADDR, LED_SET_REG, 0);
// delay(1);
//while(1);
wrSensorReg8_8(STM32_2_SLAVE_ADDR, NOTE_REG, 5);
}
