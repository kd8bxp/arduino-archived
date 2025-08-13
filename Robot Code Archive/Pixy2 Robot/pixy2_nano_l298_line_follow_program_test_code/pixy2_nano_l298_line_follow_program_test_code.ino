/*
Copyright (c) 2019 LeRoy Miller

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
#include <Pixy2.h>
#include <PIDLoop.h>

#define ldir 4 
#define lpwm 5 //10 
#define rdir 7 //5 
#define rpwm 6 //9
#define armPinA 2 
#define armPinB 3 

#define ZUMO_FAST       150 //200
#define ZUMO_SLOW       100 //150
#define X_CENTER         (pixy.frameWidth/2)

Pixy2 pixy;
PIDLoop headingLoop(5000, 0, 0, false);

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
pinMode(ldir, OUTPUT);
pinMode(rdir, OUTPUT);  
pinMode(lpwm, OUTPUT);
pinMode(rpwm, OUTPUT);
  pinMode(armPinA, OUTPUT);
pinMode(armPinB, OUTPUT);
    armStop();
    stop();
    close();
     pixy.init();
  // Turn on both lamps, upper and lower for maximum exposure
  pixy.setLamp(1, 1);
  // change to the line_tracking program.  Note, changeProg can use partial strings, so for example,
  // you can change to the line_tracking program by calling changeProg("line") instead of the whole
  // string changeProg("line_tracking")
  pixy.changeProg("line");
  // look straight and down
  pixy.setServos(500, 1000);
  }

void loop(){ 
 int8_t res;
  int32_t error; 
  int left, right;
  char buf[96];

  // Get latest data from Pixy, including main vector, new intersections and new barcodes.
  res = pixy.line.getMainFeatures();

  // If error or nothing detected, stop motors
  if (res<=0) {
    stop();
    Serial.print("stop ");
    Serial.println(res);
    return;
  }
  // We found the vector...
  if (res&LINE_VECTOR)
  {
    // Calculate heading error with respect to m_x1, which is the far-end of the vector,
    // the part of the vector we're heading toward.
    error = (int32_t)pixy.line.vectors->m_x1 - (int32_t)X_CENTER;

    pixy.line.vectors->print();

    // Perform PID calcs on heading error.
    headingLoop.update(error);
    // separate heading into left and right wheel velocities.
    left = headingLoop.m_command;
    right = -headingLoop.m_command;

    // If vector is heading away from us (arrow pointing up), things are normal.
    if (pixy.line.vectors->m_y0 > pixy.line.vectors->m_y1)
    {
      // ... but slow down a little if intersection is present, so we don't miss it.
      if (pixy.line.vectors->m_flags&LINE_FLAG_INTERSECTION_PRESENT)
      {
        left += ZUMO_SLOW;
        right += ZUMO_SLOW;
      }
      else // otherwise, pedal to the metal!
      {
        left += ZUMO_FAST;
        right += ZUMO_FAST;
      }    
    }
    else  // If the vector is pointing down, or down-ish, we need to go backwards to follow.
    {
      left -= ZUMO_SLOW;
      right -= ZUMO_SLOW;  
    } 
    //motors.setLeftSpeed(left);
    //motors.setRightSpeed(right);
    forward(left,right);
  }
  // If barcode, acknowledge with beep, and set left or right turn accordingly. 
  // When calling setNextTurn(), Pixy will "execute" the turn upon the next intersection, 
  // making the left or right branch in the intersection the new main vector, depending on 
  // the angle passed to setNextTurn(). The robot will then follow the branch.  
  // If the turn is not set, Pixy will choose the straight(est) path by default, but 
  // the default turn can be changed too by calling setDefaultTurn(). The default turn
  // is normally 0 (straight).   
  if (res&LINE_BARCODE)
  {
                                                                                                                                                                                                                                                                                                                                    pixy.line.barcodes->print();
    // code==0 is our left-turn sign
    if (pixy.line.barcodes->m_code==0)
      pixy.line.setNextTurn(90); // 90 degrees is a left turn 
    // code==5 is our right-turn sign
    else if (pixy.line.barcodes->m_code==5)
      pixy.line.setNextTurn(-90); // -90 is a right turn 
  }
}

void stop() {
  analogWrite(lpwm, 0);
  analogWrite(rpwm, 0);
  digitalWrite(ldir, 0);
  digitalWrite(rdir, 0);
}

void forward(int lspd, int rspd) {
  analogWrite(lpwm, 255 - lspd);
  digitalWrite(ldir, HIGH);
  analogWrite(rpwm, 255 - rspd);
  digitalWrite(rdir, HIGH);
}

void back(int lspd, int rspd) {
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, LOW);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, LOW);
}

void armStop() {
  //analogWrite(armPinB, 0);
  digitalWrite(armPinB, 0);
  digitalWrite(armPinA, 0);
  
}

void close() {
  //analogWrite(lpwm, 50);
  digitalWrite(armPinB, HIGH);
  digitalWrite(armPinA, LOW);
  delay(2500);
}

void open() {
  //analogWrite(lpwm, 25);
  digitalWrite(armPinB, LOW);
  digitalWrite(armPinA, HIGH);
  delay(2500);
}


