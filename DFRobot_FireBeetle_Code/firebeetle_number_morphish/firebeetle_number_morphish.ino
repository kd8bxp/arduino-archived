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
#include "DFRobot_HT1632C.h"

#define DATA D6
#define CS D2
#define WR D7
//#define RD D8


DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);
byte digitBits[] = {
  B11111100, // 0 ABCDEF--
  B01100000, // 1 -BC-----
  B11011010, // 2 AB-DE-G-
  B11110010, // 3 ABCD--G-
  B01100110, // 4 -BC--FG-
  B10110110, // 5 A-CD-FG-
  B10111110, // 6 A-CDEFG-
  B11100000, // 7 ABC-----
  B11111110, // 8 ABCDEFG-
  B11110110, // 9 ABCD_FG-
};


void setup() {
  Serial.begin(9600);
display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setPwm(8);
  display.setFont(FONT8X4);
  
}

void loop() {

  //display.print("0 0"); //123456789");
  //Zero
  display.clrPixel(0,0);
  display.clrPixel(0,1);
  display.clrPixel(0,2);
  display.clrPixel(0,3);
  display.clrPixel(0,4);
  display.clrPixel(0,5);
  display.clrPixel(0,6);
  display.clrPixel(0,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(1,0);
  display.setPixel(1,1);
  display.setPixel(1,2);
  display.setPixel(1,3);
  display.setPixel(1,4);
  display.setPixel(1,5);
  display.setPixel(1,6);
  display.clrPixel(1,7);
  display.writeScreen();
  delay(100);
  display.setPixel(2,0);
  display.clrPixel(2,1);
  display.setPixel(2,2);
  display.setPixel(2,3);
  display.clrPixel(2,4);
  display.clrPixel(2,5);
  display.clrPixel(2,6);
  display.setPixel(2,7);
  display.writeScreen();
  delay(100);
  display.setPixel(3,0);
  display.clrPixel(3,1);
  display.clrPixel(3,2);
  display.clrPixel(3,3);
  display.setPixel(3,4);
  display.setPixel(3,5);
  display.clrPixel(3,6);
  display.setPixel(3,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(0,0);
  display.setPixel(4,1);
  display.setPixel(4,2);
  display.setPixel(4,3);
  display.setPixel(4,4);
  display.setPixel(4,5);
  display.setPixel(4,6);
  display.clrPixel(4,7);
  display.writeScreen();
  delay(100);

  
delay(1000);
  //One
  display.clrPixel(0,0);
  display.clrPixel(0,1);
  display.clrPixel(0,2);
  display.clrPixel(0,3);
  display.clrPixel(0,4);
  display.clrPixel(0,5);
  display.clrPixel(0,6);
  display.clrPixel(0,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(1,0);
  display.clrPixel(1,1);
  display.clrPixel(1,2);
  display.clrPixel(1,3);
  display.clrPixel(1,4);
  display.clrPixel(1,5);
  display.clrPixel(1,6);
  display.clrPixel(1,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(2,0);
  display.setPixel(2,1);
  display.clrPixel(2,2);
  display.clrPixel(2,3);
  display.clrPixel(2,4);
  display.clrPixel(2,5);
  display.clrPixel(2,6);
  display.setPixel(2,7);
  display.writeScreen();
  delay(100);
  display.setPixel(3,0);
  display.setPixel(3,1);
  display.setPixel(3,2);
  display.setPixel(3,3);
  display.setPixel(3,4);
  display.setPixel(3,5);
  display.setPixel(3,6);
  display.setPixel(3,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(4,0);
  display.clrPixel(4,1);
  display.clrPixel(4,2);
  display.clrPixel(4,3);
  display.clrPixel(4,4);
  display.clrPixel(4,5);
  display.clrPixel(4,6);
  display.setPixel(4,7);
  display.writeScreen();
  
    delay(1000);

  //Two
  display.clrPixel(4,0);
  display.setPixel(4,1);
  display.setPixel(4,2);
  display.setPixel(4,3);
  display.clrPixel(4,4);
  display.clrPixel(4,5);
  display.clrPixel(4,6);
  display.setPixel(4,7);
  display.writeScreen();
  delay(100);
  display.setPixel(3,0);
  display.clrPixel(3,1);
  display.clrPixel(3,2);
  display.clrPixel(3,3);
  display.setPixel(3,4);
  display.clrPixel(3,5);
  display.clrPixel(3,6);
  display.setPixel(3,7);
  display.writeScreen();
  delay(100);
  display.setPixel(2,0);
  display.clrPixel(2,1);
  display.clrPixel(2,2);
  display.clrPixel(2,3);
  display.clrPixel(2,4);
  display.setPixel(2,5);
  display.clrPixel(2,6);
  display.setPixel(2,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(1,0);
  display.setPixel(1,1);
  display.clrPixel(1,2);
  display.clrPixel(1,3);
  display.clrPixel(1,4);
  display.clrPixel(1,5);
  display.setPixel(1,6);
  display.setPixel(1,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(0,0);
  display.clrPixel(0,1);
  display.clrPixel(0,2);
  display.clrPixel(0,3);
  display.clrPixel(0,4);
  display.clrPixel(0,5);
  display.clrPixel(0,6);
  display.clrPixel(0,7);
  display.writeScreen();
  delay(100);  

  delay(1000);

  //Three
  display.clrPixel(0,0);
  display.clrPixel(0,1);
  display.clrPixel(0,2);
  display.clrPixel(0,3);
  display.clrPixel(0,4);
  display.clrPixel(0,5);
  display.clrPixel(0,6);
  display.clrPixel(0,7);
  display.writeScreen();
  delay(100);  
  display.clrPixel(1,0);
  display.setPixel(1,1);
  display.clrPixel(1,2);
  display.clrPixel(1,3);
  display.clrPixel(1,4);
  display.clrPixel(1,5);
  display.setPixel(1,6);
  display.clrPixel(1,7);
  display.writeScreen();
  delay(100);
  display.setPixel(2,0);
  display.clrPixel(2,1);
  display.clrPixel(2,2);
  display.clrPixel(2,3);
  display.setPixel(2,4);
  display.clrPixel(2,5);
  display.clrPixel(2,6);
  display.setPixel(2,7);
  display.writeScreen();
  delay(100);
  display.setPixel(3,0);
  display.clrPixel(3,1);
  display.clrPixel(3,2);
  display.clrPixel(3,3);
  display.setPixel(3,4);
  display.clrPixel(3,5);
  display.clrPixel(3,6);
  display.setPixel(3,7);
  display.writeScreen();
  delay(100);
  display.clrPixel(4,0);
  display.setPixel(4,1);
  display.setPixel(4,2);
  display.setPixel(4,3);
  display.clrPixel(4,4);
  display.setPixel(4,5);
  display.setPixel(4,6);
  display.clrPixel(4,7);
  display.writeScreen();
  delay(100);

  delay(1000);
  
 // while(1);
  

}
