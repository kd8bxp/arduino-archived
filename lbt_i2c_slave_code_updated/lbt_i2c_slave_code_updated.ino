/*
 * I2C interface for Little Buddy Talker
 * Proof of Concept: version 1.0 April 14, 2018 - LeRoy Miller
 * v1.0.1 Apr 15, 2018 - updated code for v4.0.0 of Library
 * added status registers
 *  Based on code from:
 * https://www.arduino.cc/en/Tutorial/MasterWriter
 * and
 * http://dsscircuits.com/articles/arduino-i2c-slave-guide
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

/* This code is loaded on a Arduino Nano or Mini. (tested on Nano)
 *  The Little Buddy Talker is hooked to the Nano
 *  DI to pin D11
 *  SC to pin D13
 *  CS to pin D10 
 */

#include <Wire.h>
#include "LBTI2CSlave.h"

#define CS_PIN 10

LBTI2CSlave Word100(CS_PIN); //cs pin

/*I2C Command Registers
 * 0x00 Status
 * 0x01 say
 * 0x02 sayNumber 
 * 0x03 sayHours
 * 0x04 sayMinutes
 * 0x05 setAMPM
 * 0x06 setDelay
 */

#define ADDRESS 0x08 //my I2C address
int c, i, x, upDateStatus;

void setup() {
Word100.begin();  
Serial.begin(9600); //setup serial interface for debug
Wire.begin(ADDRESS);
Wire.onReceive(receiveEvent);
Wire.onRequest(requestEvent);
}

void loop() {
  }

void receiveEvent(int howMany) {
 
c = Wire.read(); // receive byte as a character
x = Wire.read(); 

//Serial.println(x,HEX);

/*I2C Registers
 * 0x00 N/A - Posiable Status
 * 0x01 say
 * 0x02 sayNumber
 * 0x03 sayHours
 * 0x04 sayMinutes
 * 0x05 setAMPM
 * 0x06 setDelay
 */

switch(c) {
  case 0x00:
  upDateStatus = x;
  break;
  case 0x01:
  Word100.say(x);
  break;
  case 0x02:
  Word100.sayNumber(x);
  break;
  case 0x03:
  Word100.sayHours(x);
  break;
  case 0x04:
  Serial.println(x);
  Word100.sayMinutes(x);
  break;
  case 0x05:
  Word100.setAMPM(x);
  break;
  case 0x06:
  x = getDelayoverI2C();
  Word100.setDelay(x);
  break;
  default:
  break;
        }
  }

/*Status Registers
0x01 AMPM set
0x02 Speaking
0x03 Delay Time
*/

void requestEvent() {
  switch(upDateStatus) {
    case 0x01:
    i = Word100.getAMPM();
    Wire.write(i);
    break;
    case 0x02:
    i = Word100.getSpeaking();
    Wire.write(i);
    break;
    case 0x03:
    i = Word100.getDelay();
    Serial.print("Get Delay: ");
    Serial.println(i);
    Wire.write((byte*)&i,2);
    break;
    default:
    Wire.write(0xFF); //255 Error
    break;
  }
}

int getDelayoverI2C() {
  x = 0;
  if (Wire.available() >= 2) {
    for (int xx=0;xx<2;xx++) {
      x += Wire.read() << (xx * 8);
    }
  }
  Serial.print("Got: ");
  Serial.println(x);
  return x;
}

