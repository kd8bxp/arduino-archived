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

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);


uint8_t blankBinary[] = {
 B11100000,
  B10100000,
  B11100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
}; 

uint8_t fullBinary[] = {
  B00011000,
  B00011000,
  B00111100,
  B11111111,
  B11111111,
  B00111100,
  B00011000,
  B00011000
};

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  display.begin();
  display.isLedOn(true);
  display.clearScreen();
  drawBlank();
 // hourOne();
 // hourTwo();
 // hourFour();
 // hourEight();
//Serial.print(59,BIN);
}

void loop() {

int hour = 12;
int minute = 34;

for (hour = 1; hour<12; hour++) {

if (hour >= 13) { hour = hour -12; }
if (hour == 0) {hour = 12;}

int tempHour = hour;
byte mask = 0x0001;
if (tempHour & mask) {
  display.setPixel(17,1); } else {
    display.clrPixel(17,1);
  }
if (tempHour>>1 & mask) {
  display.setPixel(13,1); } else {
    display.clrPixel(13,1);
  }
if (tempHour>>2 & mask) {
  display.setPixel(9,1); } else {
    display.clrPixel(9,1);
  }
if (tempHour>>3 & mask) {
  display.setPixel(5,1); } else {
    display.clrPixel(5,1);
  }
Serial.println(hour);
display.writeScreen();

delay(500);
}

for (minute = 0; minute < 59; minute++) {
int tempMinute = minute;
byte mask = 0x0001;
if (tempMinute & mask) {
  display.setPixel(21,5); } else {
    display.clrPixel(21,5); }
if (tempMinute>>1 & mask) {
  display.setPixel(17,5); } else {
    display.clrPixel(17,5); }
if (tempMinute>>2 & mask) {
  display.setPixel(13,5); } else {
    display.clrPixel(13,5); }
if (tempMinute>>3 & mask) {
  display.setPixel(9,5); } else {
    display.clrPixel(9,5); }
if (tempMinute>>4 & mask) {
  display.setPixel(5,5); } else {
    display.clrPixel(5,5); }
if (tempMinute>>5 & mask) {
  display.setPixel(1,5); } else {    
    display.clrPixel(1,5); }
display.writeScreen();

for (int x = 0; x<3; x++) {
for (int seconds=0;seconds<24;seconds++) {

//int tempSeconds = seconds;
//if (seconds > 24) {tempSeconds = 25+seconds;};
 display.setPixel(24-seconds,7);
 display.writeScreen();
 delay(25);
 display.clrPixel(24-seconds+1,7);
 display.writeScreen();
 delay(25);
}

for (int seconds=0;seconds<24;seconds++) {
 // int tempSeconds = seconds;
 // if
 display.clrPixel(0+seconds-1,7);
 display.writeScreen();
 delay(25);
 display.setPixel(0+seconds+1,7);
 display.writeScreen();
 delay(25);
}

}
}
    
while (1){}

}

void hourOne() {
  display.setPixel(17,1);
  display.writeScreen();
}

void hourTwo() {
  display.setPixel(13,1);
  display.writeScreen();
}

void hourFour() {
  display.setPixel(9,1);
  display.writeScreen();
}

void hourEight() {
  display.setPixel(5,1);
  display.writeScreen();
}

void drawBlank() {
 
  display.drawImage(blankBinary,8,8,4,0,0);
  display.drawImage(blankBinary,8,8,8,0,0);
  display.drawImage(blankBinary,8,8,12,0,0);
  display.drawImage(blankBinary,8,8,16,0,0);
  
  display.drawImage(blankBinary,8,8,0,4,0);
  display.drawImage(blankBinary,8,8,4,4,0);
  display.drawImage(blankBinary,8,8,8,4,0);
  display.drawImage(blankBinary,8,8,12,4,0);
  display.drawImage(blankBinary,8,8,16,4,0);
  display.drawImage(blankBinary,8,8,20,4,0);
  display.drawImage(blankBinary,8,8,24,4,0);
  //display.drawImage(blankBinary,8,8,8,8,0);
 display.writeScreen();
delay(5);
 
}


