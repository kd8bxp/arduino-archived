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

/*
 * 10 EStop
 * 5 Full Speed >= 90 Inches
 * 3 Half Speed < 24 Inches
 * 7 Stop and turn <= 8 inches
 */

//I2C Slave Device #8
#include <Wire.h>

#define left1 12
#define left2 11 //pwm
#define right1 10
#define right2 9 //pwm

#define topSpeed 150
#define halfSpeed 100

int cmd;

void setup() {
  Serial.begin(19200);
   Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  
stop(); //make sure motors are stopped

}

void loop() {
  switch(cmd) {
    case 7:
    //Serial.println("Turn");
    stop();
    cw(60,200);
    delay(500);
    break;
    case 3:
    //Serial.println("HalfSpeed");
    ccw(halfSpeed,halfSpeed);
    break;
     case 5:
    //Serial.println("TopSpeed");
    ccw(topSpeed,topSpeed);
    break;
    case 10:
    //Serial.println("E-Stop");
    stop();
    break;    
    default:
    stop();
    break;
     }
     //cmd = 0;
}

void cw(int lft, int rgt) { 
  //This causes the car to move backward
  digitalWrite(left1, 1);
  digitalWrite(right1, 1);
  analogWrite(left2, map(lft,0,255,255,0));
  analogWrite(right2,map(rgt,0,255,255,0));  
}

void ccw(int lft, int rgt) {
  //This causes the car to move forward
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  analogWrite(left2, lft);
  analogWrite(right2, rgt);
}

void stop() {
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  analogWrite(left2, 0);
  analogWrite(right2, 0);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
     }
  cmd = Wire.read();    // receive byte as an integer
  }
