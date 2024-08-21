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

DFRobot_HT1632C ht1632c = DFRobot_HT1632C(DATA, WR,CS);


//https://www.riyas.org/2013/12/online-led-matrix-font-generator-with.html
uint8_t enterpriseFront[] = {B00001000,
B00001000,
B00001100,
B01101100,
B01101100,
B01111000,
B01111000,
B01110000};

uint8_t enterpriseBack[] = {B01100100,
B01100100,
B01111100,
B00100100,
B00100100,
B00000100,
B00000100,
B00000100};

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
}

void loop() {
  //ht1632c.clearScreen();


// for (int i=0; i<25; i++) {
for (int i=0; i<25+8; i++) {
  
  ht1632c.clearScreen();
ht1632c.drawImage(enterpriseFront,8,8,25-i,0,0);
ht1632c.drawImage(enterpriseBack, 8,8,25+8-i, 0, 0);

ht1632c.writeScreen();
delay(250);
 
 }
 
}
