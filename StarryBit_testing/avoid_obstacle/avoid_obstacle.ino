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

#define left1 14
#define left2 16 //pwm
#define right1 13
#define right2 15 //pwm
#define ir 0 //IR obstacle avoidance sensor

void setup() {
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(ir, INPUT);  
drive(0,0);

}

void loop() {
  avoid();

}

void drive(int lft, int rgt) {
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  if (lft < 0) {
    digitalWrite(left1, 1);
    lft = 1023 + lft;
  }
  if (rgt < 0) {
    digitalWrite(right1, 1);
    rgt = 1023 + rgt;
  }
  analogWrite(left2, lft);
  analogWrite(right2, rgt);
  
}

void avoid() {
  int IR = analogRead(ir);
  if (IR > 250) { 
      drive(500,500); 
  } else {
    drive(-300,50);
    delay(500);
  }
}

