/*
 * I2C interface for Little Buddy Talker
 * version 1.0 April 14, 2018 - LeRoy Miller
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

#include <Wire.h>
#include "Word100LBT.h"

#define CS_PIN 10

Word100lbt Word100(CS_PIN); //cs pin

/*I2C Registers
 * 0x00 N/A - Posiable Status
 * 0x01 say
 * 0x02 sayNumber
 * 0x03 sayHours
 * 0x04 sayMinutes
 * 0x05 setAMPM
 * 0x06 setDelay
 */

#define ADDRESS 0x08 //my I2C address
int c;

void setup() {
Word100.begin();  
Serial.begin(9600); //setup serial interface for debug
Wire.begin(ADDRESS);
Wire.onReceive(receiveEvent);
}

void loop() {
  }

void receiveEvent(int howMany) {
 
while (1 < Wire.available()) { // loop through all but the last
    c = Wire.read(); // receive byte as a character
    Serial.println(c,HEX);   
    }
int x = Wire.read();
Serial.println(x,HEX);
//int y = Wire.read();
//Serial.println(y,HEX);

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
  Word100.sayMinutes(x);
  break;
  case 0x05:
  Word100.setAMPM(x);
  break;
  case 0x06:
  Word100.setDelay(x);
  default:
  break;
        }
  }

