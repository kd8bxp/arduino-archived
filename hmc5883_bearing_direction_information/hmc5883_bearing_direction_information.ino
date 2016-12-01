/*
 Uses HMC5883L to get earths magnetic field in x,y and z axis
 Displays direction as angle between 0 and 359 degrees
*/

#include <Wire.h> //I2C Arduino Library

const int hmc5883Address = 0x1E; //0011110b, I2C 7bit address of HMC5883
const byte hmc5883ModeRegister      = 0x02;
const byte hmcContinuousMode        = 0x00;
const byte hmcDataOutputXMSBAddress = 0x03;

#define DEC_ANGLE -1.047

void setup(){
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  Wire.begin();
 
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(hmc5883Address); //open communication with HMC5883
  Wire.write(hmc5883ModeRegister); //select mode register
  Wire.write(hmcContinuousMode);   //continuous measurement mode
  Wire.endTransmission();
}

void loop(){
 
  int x,y,z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(hmc5883Address);
  Wire.write(hmcDataOutputXMSBAddress); //select register 3, X MSB register
  Wire.endTransmission();
 
 
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(hmc5883Address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
 
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.print(z);
 
 float heading = atan2(y, x);
 heading += DEC_ANGLE;
 
 if (heading < 0) heading += 2*PI;
 if (heading > 2*PI) heading -= 2*PI;
 
 float headingDegrees = heading * 180/M_PI;
 
 // int angle = atan2(-y , x) / M_PI * 180; // angle is atan(-y/x)
 // if(angle < 0)
 //    angle = angle  + 360; // angle from 0 to 359 instead of plus/minus 180
  Serial.print(" Direction = ");
  Serial.println(headingDegrees);
 
  delay(250);
}