/*  ********************************************* 
 *  SparkFun_MAG3110_Basic
 *  Triple Axis Magnetometer Breakout - MAG3110 
 *  Hook Up Guide Example 
 *  
 *  Utilizing Sparkfun's MAG3110 Library
 *  A basic sketch that reads x y and z readings
 *  from the MAG3110 sensor
 *  
 *  George B. on behalf of SparkFun Electronics
 *  Created: Sep 22, 2016
 *  Updated: n/a
 *  
 *  Development Environment Specifics:
 *  Arduino 1.6.7
 *  
 *  Hardware Specifications:
 *  SparkFun MAG3110
 *  Bi-directional Logic Level Converter
 *  Arduino Micro
 *  
 *  This code is beerware; if you see me (or any other SparkFun employee) at the
 *  local, and you've found our code helpful, please buy us a round!
 *  Distributed as-is; no warranty is given.
 *  *********************************************/

#include <SparkFun_MAG3110.h>

MAG3110 mag = MAG3110(); //Instantiate MAG3110

void setup() {
  Serial.begin(9600);

  mag.initialize(); //Initializes the mag sensor
  mag.start();      //Puts the sensor in active mode
}

void loop() {

  int x, y, z;
  //Only read data when it's ready
  if(mag.dataReady()) {
    //Read the data
    mag.readMag(&x, &y, &z);
  
    //Serial.print("X: ");
    //Serial.print(x);
    //Serial.print(", Y: ");
    //Serial.print(y);
    //Serial.print(", Z: ");
    //Serial.print(z);
  Serial.print(" Heading: ");
  //Serial.println((atan2(x,z)*180)); //x,z
  //Serial.println(atan2(y,z)*180);
  //Serial.println(((atan2(x,y)+atan2(x,z))*90));
  //Serial.println(atan(y)*360);
  Serial.println(atan2(x,y)*180);
    //Serial.println("--------");

/*
y = y /57.0; //convert to rad
x = x /57.0;
float xh, yh;


  
  xh=x*cos(y)+y*sin(y)*sin(x)-z*cos(x)*sin(y);
  yh=y*cos(x)+z*sin(x);
 
  float compass=atan2((double)yh,(double)xh) * (180 / 3.14) -90; // angle in degrees
  if (compass>0){compass=compass-360;}
  compass=360+compass;
  Serial.println(compass);
*/

 //Serial.println((180/3.14) * (atan(z) + atan(x)+ atan(y)));
    
    delay(1000);
  }


}
