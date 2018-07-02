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


uint8_t COLONON[] = {
  B00000000,
  B00010100,
  B00000000,
  B00000000
}; //:

uint8_t COLONOFF[] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

//zero
 uint8_t zerobyte0[] = {B01111110,B00000000,B00000000,B00000000};
 uint8_t zerobyte1[] = {B10110001,B00000000,B00000000,B00000000};
 uint8_t zerobyte2[] = {B10001101,B00000000,B00000000,B00000000};
 uint8_t zerobyte3[] = {B01111110,B00000000,B00000000,B00000000};

//one
 uint8_t onebyte0[] = {B01000001,B00000000,B00000000,B00000000};
 uint8_t onebyte1[] = {B11111111,B00000000,B00000000,B00000000};
 uint8_t onebyte2[] = {B00000001,B00000000,B00000000,B00000000};
 uint8_t onebyte3[] = {B00000000,B00000000,B00000000,B00000000};

//two
 uint8_t twobyte0[] = {B01000011,B00000000,B00000000,B00000000};
 uint8_t twobyte1[] = {B10000101,B00000000,B00000000,B00000000};
 uint8_t twobyte2[] = {B10001001,B00000000,B00000000,B00000000};
 uint8_t twobyte3[] = {B01110001,B00000000,B00000000,B00000000};

//three
B01000010,B00000000,B00000000,B00000000}; 
B10001001,B00000000,B00000000,B00000000};
B10001001,B00000000,B00000000,B00000000};
B01110110,B00000000,B00000000,B00000000};

 //four
B00011100,B00000000,B00000000,B00000000};
 B00100100,B00000000,B00000000,B00000000};
 B01001111,B00000000,B00000000,B00000000};
B10000100,B00000000,B00000000,B00000000};

 //five
 ,B00000000,B00000000,B00000000};
 ,B00000000,B00000000,B00000000};
 ,B00000000,B00000000,B00000000};
 ,B00000000,B00000000,B00000000};

 //six
 ,B00000000,B00000000,B00000000};
 ,B00000000,B00000000,B00000000};
 ,B00000000,B00000000,B00000000};
 ,B00000000,B00000000,B00000000};

//seven
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};

//eight
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};

//nine
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};
,B00000000,B00000000,B00000000};

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
}

void loop() {
 ht1632c.clearScreen();
 
 //digit 1
 updateDisplay(twobyte0,0,1);
 updateDisplay(twobyte1,1,1);
 updateDisplay(twobyte2,2,1);
 updateDisplay(twobyte3,3,1);
 
 //digit 2
 updateDisplay(zerobyte0,0,2);
 updateDisplay(zerobyte1,1,2);
 updateDisplay(zerobyte2,2,2);
 updateDisplay(zerobyte3,3,2);
 
 //digit 3
 updateDisplay(twobyte0,0,3);
 updateDisplay(twobyte1,1,3);
 updateDisplay(twobyte2,2,3);
 updateDisplay(twobyte3,3,3);
 //digit 4
 updateDisplay(onebyte0,0,4);
 updateDisplay(onebyte1,1,4);
 updateDisplay(onebyte2,2,4);
 updateDisplay(onebyte3,3,4);
 
for (int i=0; i<5; i++) {
  
  ht1632c.drawImage(COLONOFF,3,8,10,0,0);
  ht1632c.writeScreen();
  delay(1000);
  ht1632c.drawImage(COLONON,3,8,10,0,0);
  ht1632c.writeScreen();
  delay(1000);
}
}

void updateDisplay(uint8_t *DIGIT, int number, int loc) {
  int start, offset;
  if (loc == 1) {start = 0 + number; offset = 0;} //3
  if (loc == 2) {start = 5 + number; offset = 5;} //8
  if (loc == 3) {start = 10 + number; offset = 12;} //13
  if (loc == 4) {start = 15 + number; offset = 17;} //17
  for (int i=start; i<25-number; i++) {
  ht1632c.drawImage(DIGIT,4,8,25+offset-i,0,0);
  ht1632c.writeScreen();
delay(5);
 }
}

