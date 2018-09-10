/*
 * Losely based on code by pimoroni for the scroll-phat-hd
 * Specifically losely based on the trigonometric functions 
 * to create a simple animation.
 * (python code) 
 * https://learn.pimoroni.com/tutorial/sandyj/getting-started-with-scroll-phat-hd
 * 
 * By changing how b is checked animation patterns can be changed.
 * By changing what is multiplied to t (millis()) animation patterns are changed. 
 *    * Patterns will change over time the longer the Firebeetle ESP32 is running.
 * by changing the delay, will effect how the display looks.  
 * 
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

#if defined( ESP_PLATFORM ) || defined( ARDUINO_ARCH_FIREBEETLE8266 )  //FireBeetle-ESP32 FireBeetle-ESP8266
#define DATA D6
#define CS D2
#define WR D7
//#define RD D8
#else
#define DATA 6
#define CS 2
#define WR 7
//#define RD 8
#endif

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);

float t;
int x,y,r;
float b;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setFont(FONT8X4);
  delay(500);
  x = -1;
  y = -1;
  r = random(analogRead(A0),analogRead(A1));
}

void loop() {

if (x > 24) {x = -1;}
if (y > 8) {y = -1;} 
x = x+1;
y = y+1;
  float t = millis() * 1100;
  
    
    //b = -1 * (sin(x+y+t)) + (cos(x+y+t));
    b = (sin(x+y+t)) + (cos(x+y+t));
    
    if (b <= 0) {
    display.setPixel(x,y); } else {
        display.clrPixel(x,y);
        }  
        display.writeScreen();  
      delay(5);
}
