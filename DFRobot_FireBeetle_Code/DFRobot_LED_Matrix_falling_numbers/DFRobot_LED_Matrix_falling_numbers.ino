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

uint8_t ZERO[] = {
  B01111110,
  B10001101,
  B10110001,
  B01111110
}; //0

uint8_t ONE[] = {
  B01000001,
  B11111111,
  B00000001,
  B00000000
}; //1

uint8_t TWO[] = {
  B01000011,
  B10000101,
  B10001001,
  B01110001
}; //2

uint8_t THREE[] = {
  B01000010,
  B10001001,
  B10001001,
  B01110110
}; //3

uint8_t FOUR[] = {
  B00011100,
  B00100100,
  B01001111,
  B10000100
};

uint8_t FIVE[] = {
  B11110001,
  B10010001,
  B10010001,
  B10001110
};

uint8_t SIX[] = {
  B01111110,
  B10001001,
  B10001001,
  B01000110
};

uint8_t SEVEN[] = {
  B10000000,
  B10000111,
  B10011000,
  B11100000
};

uint8_t EIGHT[] = {
  B01110110,
  B10001001,
  B10001001,
  B01110110
};

uint8_t NINE[] = {
  B01110010,
  B10001001,
  B10001001,
  B01111110
};

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
}

void loop() {
  
 for (int i=3; i<25; i++) {
  ht1632c.clearScreen();
  ht1632c.drawImage(ONE,4,8,25-i,0,0);
  ht1632c.drawImage(SEVEN,4,8,25+5-i,0,0);
  ht1632c.drawImage(COLONON,4,8,25+9-i,0,0);
  ht1632c.drawImage(THREE,4,8,25+12-i,0,0);
  ht1632c.drawImage(NINE,4,8,25+17-i,0,0);
  ht1632c.writeScreen();
delay(60);

 }
 
for (int i=0; i<30; i++) {
  delay(1000);
  ht1632c.drawImage(COLONOFF,3,8,10,0,0);
  ht1632c.writeScreen();
  delay(1000);
  ht1632c.drawImage(COLONON,3,8,10,0,0);
  ht1632c.writeScreen();
}

}
