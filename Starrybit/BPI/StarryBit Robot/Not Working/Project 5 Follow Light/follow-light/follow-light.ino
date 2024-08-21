/*
Copyright (c) 2021 LeRoy Miller

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

/*Follow the Light based from the python code provided
 * modified to stop (vs forward) when sensor reading is 
 * between 400 and 750. 
 */

#include "src/ESP32_AnalogWrite/analogWrite.h"
//ESP32 AnalogWrite https://www.arduino.cc/reference/en/libraries/esp32-analogwrite/

#define left1 19 //14
#define left2 5 //16 //pwm
#define right1 18 //13
#define right2 23 //15 //pwm
#define ir 25 //0 //IR obstacle avoidance sensor
#define photo 33 //2  //photo-resistor is an array on pin 2

void setup() {
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(ir, INPUT); 
  pinMode(photo, INPUT_PULLUP); 
stop();

}

void loop() {
  //Arduino PWM is from 0 to 255
  //ccw (counter clockwise) will move the car forward
  //in the python examples this is positive numbers.
  //cw (clockwise) will move the car backward
  //in the python examples this is the negitive numbers.
  //PWM for left, right motor
  findLight();
delay(200);
}

void cw(int lft, int rgt) { 
  //This causes the car to move backward
  digitalWrite(left1, 1);
  digitalWrite(right1, 1);
  analogWrite(left2, map(lft,50,255,50,255));
  analogWrite(right2, map(rgt,50,255,50,255));  
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


void findLight() {
  int readPhoto = analogRead(photo);
  if (readPhoto < 400) {
    //left 
      ccw(50,150); 
  } 
  else if (readPhoto > 750) {
    //right
    ccw(150,50);
      }
  else {
    
    //ccw(125,125);
    stop();    
  }
}
