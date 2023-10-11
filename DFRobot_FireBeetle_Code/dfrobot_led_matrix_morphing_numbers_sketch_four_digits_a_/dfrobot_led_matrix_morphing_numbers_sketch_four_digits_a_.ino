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

#include "src/DFRobot_HT1632C/DFRobot_HT1632C.h"

#define DATA D6
#define CS D2
#define WR D7
//#define RD D8


DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);


void setup() {
  Serial.begin(9600);
  display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setPwm(8);
  display.setFont(FONT8X4);
  
}

void loop() {

  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  zero(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToOne(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToTwo(i);
 // delay(500);
  }
  for (int i = 3; i>=0; i--) {
 // for (int i = 0; i<4; i++) {
  morphToThree(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToFour(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToFive(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToSix(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToSeven(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToEight(i);
  //delay(500);
  }
  for (int i = 3; i>=0; i--) {
  //for (int i = 0; i<4; i++) {
  morphToNine(i);
  //delay(500);
  
  }
}


// place 0 - 0 offset
// place 1 - 6 offset
// place 2 - 13 offset
// place 3 - 19 offset

void zero(int place) {
  int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  //Zero
  display.clrPixel(offset+0,0);
  display.clrPixel(offset+0,1);
  display.clrPixel(offset+0,2);
  display.clrPixel(offset+0,3);
  display.clrPixel(offset+0,4);
  display.clrPixel(offset+0,5);
  display.clrPixel(offset+0,6);
  display.clrPixel(offset+0,7);
  display.writeScreen();
  delay(75);
  display.clrPixel(offset+1,0);
  display.setPixel(offset+1,1);
  display.setPixel(offset+1,2);
  display.setPixel(offset+1,3);
  display.setPixel(offset+1,4);
  display.setPixel(offset+1,5);
  display.setPixel(offset+1,6);
  display.clrPixel(offset+1,7);
  display.writeScreen();
  delay(75);
  display.setPixel(offset+2,0);
  display.clrPixel(offset+2,1);
  display.clrPixel(offset+2,2);
  display.clrPixel(offset+2,3);
  display.clrPixel(offset+2,4);
  display.clrPixel(offset+2,5);
  display.clrPixel(offset+2,6);
  display.setPixel(offset+2,7);
  display.writeScreen();
  delay(75);
  display.setPixel(offset+3,0);
  display.clrPixel(offset+3,1);
  display.clrPixel(offset+3,2);
  display.clrPixel(offset+3,3);
  display.clrPixel(offset+3,4);
  display.clrPixel(offset+3,5);
  display.clrPixel(offset+3,6);
  display.setPixel(offset+3,7);
  display.writeScreen();
  delay(75);
  display.clrPixel(offset+4,0);
  display.setPixel(offset+4,1);
  display.setPixel(offset+4,2);
  display.setPixel(offset+4,3);
  display.setPixel(offset+4,4);
  display.setPixel(offset+4,5);
  display.setPixel(offset+4,6);
  display.clrPixel(offset+4,7);
  display.writeScreen();
  delay(75);

}

void morphToOne(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  for (int i=0; i<4; i++) {
  display.setPixel(offset+1+i,0);
  display.setPixel(offset+1+i,1);
  display.setPixel(offset+1+i,2);
  display.setPixel(offset+1+i,3);
  display.setPixel(offset+1+i,4);
  display.setPixel(offset+1+i,5);
  display.setPixel(offset+1+i,6);
  display.setPixel(offset+1+i,7);
  display.writeScreen();
  delay(75);
  display.clrPixel(offset+1+i,0);
  display.clrPixel(offset+1+i,1);
  display.clrPixel(offset+1+i,2);
  display.clrPixel(offset+1+i,3);
  display.clrPixel(offset+1+i,4);
  display.clrPixel(offset+1+i,5);
  display.clrPixel(offset+1+i,6);
  display.clrPixel(offset+1+i,7);
  display.writeScreen();
  delay(50);
  }
  display.setPixel(offset+4,0);
  display.setPixel(offset+4,1);
  display.setPixel(offset+4,2);
  display.setPixel(offset+4,3);
  display.setPixel(offset+4,4);
  display.setPixel(offset+4,5);
  display.setPixel(offset+4,6);
  display.setPixel(offset+4,7);
  display.writeScreen();
  delay(75);
}

void morphToTwo(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  display.setPixel(offset+4,0);
  display.setPixel(offset+4,1);
  display.setPixel(offset+4,2);
  display.setPixel(offset+4,3);
  display.clrPixel(offset+4,4);
  display.clrPixel(offset+4,5);
  display.clrPixel(offset+4,6);
  display.setPixel(offset+4,7);
  delay(75);
  for (int i=0; i<3; i++) {
    display.setPixel(offset+3-i,0);
    display.clrPixel(offset+3-i,1);
    display.clrPixel(offset+3-i,2);
    display.setPixel(offset+3-i,3);
    display.setPixel(offset+3-i,4);
    display.setPixel(offset+3-i,5);
    display.setPixel(offset+3-i,6);
    display.setPixel(offset+3-i,7);
    display.writeScreen();
    delay(75);
    display.clrPixel(offset+3-i,4);
    display.clrPixel(offset+3-i,5);
    display.clrPixel(offset+3-i,6);
    display.writeScreen();
    delay(50);
  }
  display.setPixel(offset+1,0);
  display.clrPixel(offset+1,1);
  display.clrPixel(offset+1,1);
  display.setPixel(offset+1,3);
  display.setPixel(offset+1,4);
  display.setPixel(offset+1,5);
  display.setPixel(offset+1,6);
  display.setPixel(offset+1,7);
  display.writeScreen();
  delay(75);
}

void morphToThree(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  for (int i=0;i<4;i++) {
    display.setPixel(offset+1+i,4);
    display.setPixel(offset+1+i,5);
    display.setPixel(offset+1+i,6);
    display.setPixel(offset+1+i,7);
    display.writeScreen();
    delay(75);
    display.clrPixel(offset+1+i,4);
    display.clrPixel(offset+1+i,5);
    display.clrPixel(offset+1+i,6);
    display.writeScreen();
    delay(50);
  }
  display.setPixel(offset+4,4);
  display.setPixel(offset+4,5);
  display.setPixel(offset+4,6);
  display.clrPixel(offset+4,7);
  display.writeScreen();
  delay(75);
}

void morphToFour(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  display.setPixel(offset+4,3);
  display.setPixel(offset+4,0);
  display.clrPixel(offset+3,0);
  display.clrPixel(offset+2,0);
  display.clrPixel(offset+3,7);
  display.clrPixel(offset+2,7);
  display.clrPixel(offset+1,7);
  display.writeScreen();
  delay(75);
  display.setPixel(offset+3,1);
  display.setPixel(offset+3,6);
  display.setPixel(offset+2,6);
  display.setPixel(offset+1,6);
  display.writeScreen();
  delay(75);
  display.clrPixel(offset+3,1);
  display.setPixel(offset+2,2);
  display.clrPixel(offset+3,6);
  display.clrPixel(offset+2,6);
  display.clrPixel(offset+1,6);
  display.setPixel(offset+3,5);
  display.setPixel(offset+2,5);
  display.setPixel(offset+1,5);
  display.writeScreen();
  delay(75);
  display.clrPixel(offset+2,2);
  display.setPixel(offset+1,1);
  display.setPixel(offset+1,2);
  display.setPixel(offset+1,3);
  //display.setPixel(1,4);
  display.clrPixel(offset+3,5);
  display.clrPixel(offset+2,5);
  display.clrPixel(offset+1,5);
  display.setPixel(offset+4,4);
  //display.setPixel(2,4);
   display.writeScreen();
  delay(75);
   
}

void morphToFive(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  //display.clrPixel(4,3);
  //display.clrPixel(1,0);
  display.writeScreen();
  delay(75);
  
  //display.clrPixel(1,3);
  display.clrPixel(offset+4,2);
  display.writeScreen();
  delay(75);
  display.clrPixel(offset+4,1);
  display.clrPixel(offset+4,0);
  display.writeScreen();
  delay(75);
  display.setPixel(offset+2,0);
  display.setPixel(offset+3,0);
  display.writeScreen();
  delay(75);
  display.setPixel(offset+4,0);
  //display.clrPixel(4,7);
  display.writeScreen();
  delay(75);
  display.setPixel(offset+3,7);
  display.setPixel(offset+2,7);
  display.writeScreen();
  delay(75);
  display.setPixel(offset+1,7);
  display.writeScreen();
  delay(75);
  
}

void morphToSix(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  display.setPixel(offset+4,7);
  for (int i=0; i<3; i++) {
    display.setPixel(offset+3-i,4);
    display.setPixel(offset+3-i,5);
    display.setPixel(offset+3-i,6);
    display.setPixel(offset+3-i,7);
    display.writeScreen();
    delay(75);
    display.clrPixel(offset+3-i,4);
    display.clrPixel(offset+3-i,5);
    display.clrPixel(offset+3-i,6);
    //display.clrPixel(3-i,7);
    display.writeScreen();
    delay(50);
  }
  display.setPixel(offset+1,4);
  display.setPixel(offset+1,5);
  display.setPixel(offset+1,6);
  display.setPixel(offset+1,7);
  display.writeScreen();
  delay(75);
}

void morphToSeven(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  for (int i=0; i<4;i++) {
    display.setPixel(offset+1+i,1);
    display.setPixel(offset+1+i,2);
    display.setPixel(offset+1+i,3);
    display.setPixel(offset+1+i,4);
    display.setPixel(offset+1+i,5);
    display.setPixel(offset+1+i,6);
    display.setPixel(offset+1+i,7);
    display.writeScreen();
    delay(75);
    display.clrPixel(offset+1+i,1);
    display.clrPixel(offset+1+i,2);
    display.clrPixel(offset+1+i,3);
    display.clrPixel(offset+1+i,4);
    display.clrPixel(offset+1+i,5);
    display.clrPixel(offset+1+i,6);
    display.clrPixel(offset+1+i,7);
    display.writeScreen();
    delay(50);
  }
  display.setPixel(offset+4,1);
  display.setPixel(offset+4,2);
  display.setPixel(offset+4,3);
  display.setPixel(offset+4,4);
  display.setPixel(offset+4,5);
  display.setPixel(offset+4,6);
  display.setPixel(offset+4,7);
  display.writeScreen();
  delay(75);
}

void morphToEight(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  display.clrPixel(offset+4,0);
  display.clrPixel(offset+4,7);
  for (int i=0; i<3; i++) {
    display.setPixel(offset+3-i,1);
    display.setPixel(offset+3-i,2);
    display.setPixel(offset+3-i,3);
    display.setPixel(offset+3-i,4);
    display.setPixel(offset+3-i,5);
    display.setPixel(offset+3-i,6);
    display.setPixel(offset+3-i,7);
    display.writeScreen();
    delay(75);
    display.clrPixel(offset+3-i,1);
    display.clrPixel(offset+3-i,2);
    //display.clrPixel(3-i,3);
    display.clrPixel(offset+3-i,4);
    display.clrPixel(offset+3-i,5);
    display.clrPixel(offset+3-i,6);
    delay(50);
  }
  display.clrPixel(offset+1,0);
  display.clrPixel(offset+1,7);
  display.setPixel(offset+1,1);
  display.setPixel(offset+1,2);
  display.setPixel(offset+1,3);
  display.setPixel(offset+1,4);
  display.setPixel(offset+1,5);
  display.setPixel(offset+1,6);
  display.writeScreen();
  delay(75);
}

void morphToNine(int place) {
    int offset;
 if (place == 0) { offset = 0; }
 if (place == 1) { offset = 6; }
 if (place == 2) { offset = 13; }
 if (place == 3) { offset = 19; }
 
  display.setPixel(offset+1,0);
  display.setPixel(offset+4,0);
  for (int i=0;i<3;i++) {
    display.setPixel(offset+1+i,4);
    display.setPixel(offset+1+i,5);
    display.setPixel(offset+1+i,6);
    display.clrPixel(offset+1+i,7);
    display.writeScreen();
    delay(75);
    display.clrPixel(offset+1+i,4);
    display.clrPixel(offset+1+i,5);
    display.clrPixel(offset+1+i,6);
    display.writeScreen();
    delay(50);
  }
  display.setPixel(offset+4,7);
  display.writeScreen();
  delay(75);
}
