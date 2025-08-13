/********************************************************
 * M5Bala balance car Basic Example
 * Reading encoder and writting the motor via I2C
 ********************************************************/

#include "src/M5Stack/M5Stack.h"
#include <Wire.h>
#include "M5Bala.h"
#include "imuCalibration.h"

M5Bala m5bala(Wire);

void setup() {
  // Power ON Stabilizing...
  delay(500);
  M5.begin();

  // Init I2C
  Wire.begin();
  Wire.setClock(400000UL);  // Set I2C frequency to 400kHz
  delay(500);

  // Display info
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("M5Stack Balance Mode start");
//delay(2000);
  // Init M5Bala
  m5bala.begin();
  //m5bala.imu->calcGyroOffsets(true);
  //imu_calcGyroOffsets();
  imu_setOffsetX(-14.81); //, -1.08, 0.87);
  m5bala.setAngleOffset(-2.2);
  //m5bala.imu->setGyroOffsets(-0.02, -0.01, 0.02);
  M5.Lcd.println("Done");

}

void loop() {

  // M5Bala run
  m5bala.run();

  // M5 Loop
  M5.update();
  m5bala.rotate(-50);
  //m5bala.move(1);
  //m5bala.move(-1);
  m5bala.stop();
  //m5bala.rotate(50);
  //m5bala.move(0);
  //m5bala.stop();
}
