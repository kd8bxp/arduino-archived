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
 * Sept 1, 2018
 * This is an idea for a TIX clock using the firebeetle
 * and LED Matrix, it's incomplete the bones are here
 * but the display is just too small to do a TIX clock
 * correctly.  The display also is only one color
 * most TIX clocks use multiple color LEDs.
 * 
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

int hour = 12;
int minute = 52;
int seconds = 0;
int tempDigit1, tempDigit2, tempDigit3, tempDigit4;
int tix1 = 0;
int tix2 = 0;
int tix3 = 0;
int tix4 = 0;

uint8_t hour1pattern0[] = {
  B00000000
};

uint8_t hour1pattern1a[] = { 
  B01100000,
  B01100000
};
uint8_t hour1pattern1b[] = {
  B00110000,
  B00110000
};
uint8_t hour1pattern1c[] = {
  B00000110,
  B00000110
};

uint8_t hour1pattern2a[] = {
  B01101100,
  B01101100
};

uint8_t hour1pattern2b[] = {
  B00110110,
  B00110110
};

uint8_t hour1pattern2c[] = {
  B01101100,
  B01101100
};

void setup() {
   Serial.begin(9600);
  // put your setup code here, to run once:
  display.begin();
  display.isLedOn(true);
  display.clearScreen();

}

void loop() {
  //digit1 box
  display.drawLine(0,0,0,7);
  display.drawLine(0,7,3,7);
  display.drawLine(0,0,3,0);
  display.drawLine(3,0,3,7);
  //digit2 box
  display.drawLine(5,0,11,0);
  display.drawLine(5,7,11,7);
  display.drawLine(5,0,5,7);
  display.drawLine(11,0,11,7);
  //digit3 box
  display.drawLine(13,0,19,0);
  display.drawLine(13,7,19,7);
  display.drawLine(13,0,13,7);
  display.drawLine(19,0,19,7);
  display.writeScreen();
  
 // if (hour >= 13) { hour = hour -12; }
if (hour == 0) {hour = 12;}

if (hour <10) {
  tempDigit1 = 0;
  tempDigit2 = hour;
} else {
  tempDigit1 = hour/10;
  tempDigit2 = hour - (hour/10)*10;
}

if (minute <10) {
  tempDigit3 = 0;
  } else {
    tempDigit3 = minute/10;
    tempDigit4 = minute - (minute/10)*10;
  }

displayTixClock(tempDigit1,tempDigit2,tempDigit3,tempDigit4);
delay(1000);
seconds++;
if (seconds >= 60) {
  seconds = 0;
  minute++;
}
if (minute >= 60) {
  minute = 0;
  hour++;
}
if (hour >= 24) {
  hour = 0;
}
yield();
}

void displayTixClock(int digit1, int digit2, int digit3, int digit4) {

tix1 = random(0,3);

  if (digit1 == 0) {
    display.drawImage(hour1pattern0, 2,8,1,0,0);
  } 
  if (digit1 == 1) {
    //uint8_t* temp = (uint8_t*)hour1pattern1[tix1];
    Serial.println(tix1);
    if (tix1 == 0) {
    display.drawImage(hour1pattern1a, 2,8,1,0,0);
    }
    if (tix1 == 1) {
      display.drawImage(hour1pattern1b, 2,8,1,0,0);
    }
    if (tix1 == 2) {
      display.drawImage(hour1pattern1c, 2,8,1,0,0);
    }
    
  }
  if (digit1 == 2) {
   if (tix1 == 0) {
    display.drawImage(hour1pattern2a, 2,8,1,0,0);
    }
    if (tix1 == 1) {
      display.drawImage(hour1pattern2b, 2,8,1,0,0);
    }
    if (tix1 == 2) {
      display.drawImage(hour1pattern2c, 2,8,1,0,0);
    }
    
     
  }
  display.setPixel(2,0);
  display.setPixel(2,7);
  display.setPixel(1,0);
  display.setPixel(1,7);
  //tix1++;
 //if (tix1 >= 3) {tix1 = 0;}
 display.writeScreen();
}
