/*
Copyright (c) 2018 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/
#include <SparkFun_MAG3110.h>
#include <Wire.h>
#include <Adafruit_Microbit.h>

int i = 0;
int animationTimer = millis();
bool calibrationDone = false;

MAG3110 mag = MAG3110();
Adafruit_Microbit_Matrix microbit;

void setup() {
    Serial.begin(9600);
microbit.begin();
  //Initialize the MAG3110
  mag.initialize();
  //mag.start();
  //Start calibration
 mag.enterCalMode();
Serial.println("Calibrating Rotate Device");
microbit.print("C");
}

void loop() {
   //If we're in calibration mode
   Serial.print(".");
  if (mag.isCalibrating())
  {
    //Display the calibration screen
    if (millis() - animationTimer > 500)
    {
      i ^= 0x01; //Toggles on and off

      animationTimer = millis();


      mag.calibrate();
    }
  }

  
  //If we have just exited calibration mode
  if (mag.isCalibrated() && !calibrationDone)
  {
    
    Serial.println("Exiting calibration mode");

    delay(1000);
    calibrationDone = true;
  }
  
  if (mag.isCalibrated())
  {
    
    //As in this case, you may need to add an offset to get the proper angle
    //In my circuit, the x-axis of the MAG3110 was 90 CW of the front of my microview, so I add 90 degrees
    //The other code makes the heading pertain to 0 to 360 degrees instead of +- 180
  int heading = (int)mag.readHeading() + 90;
Serial.print("Heading: ");
Serial.println(heading);
if (heading < 45) {microbit.print("N"); }
else if (heading < 135) {microbit.print("E"); }
else if (heading < 225) {microbit.print("S"); }
else {microbit.print("W");}
delay(1000);
   }
}
