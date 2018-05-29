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
#include <Wire.h>
#include "Word100LBT.h";

#define REMOTE 0x08 //I2C address of slave device

int sentence[] = {_one, _two, _three, _four, _five, _six, _seven, _eight, _nine, _ten};
int sentence2[] = {_alert, _alert, _intruder, _in, _zone, _two};

/*I2C Registers
 * 0x00 N/A - Posiable Status
 * 0x01 say
 * 0x02 sayNumber
 * 0x03 sayHours
 * 0x04 sayMinutes
 * 0x05 setAMPM
 * 0x06 setDelay
 */
 
void setup() {
  Wire.begin();

}

#define arr_len( x ) ( sizeof ( x ) / sizeof (*x) )

void loop() {
//say the first sentence (Count to Ten)  
for (int i = 0; i < arr_len(sentence); i++) {
  say(sentence[i]);
  delay(700);
}

//Change Delay between words
setDelay(450);
//say the first sentence again with new delay
for (int i = 0; i < arr_len(sentence); i++) {
  say(sentence[i]);
  delay(700);
}

setDelay(700); //set delay back to default of 700

//say second sentance
for (int i = 0; i < arr_len(sentence2); i++) {
say(sentence2[i]);
delay(700);
}

delay(1000);

//Say a number 293003
sayNumber(293003);
delay(1000);

//say an hour 
setAMPM(1);
sayHours(13); // 1 should say 1 pm
delay(1000);
//change to 24hr time
setAMPM(0);
delay(700);
//say hour agin
sayHours(13);
delay(1000);

say(_red);
delay(700);
say(_alert);
while(1) {}
}

void sendOverI2C(int address, int intruction) {
  Wire.beginTransmission(REMOTE); //transmit to REMOTE device
  Wire.write(address);
  delay(10);
  Wire.write(intruction);
  Wire.endTransmission();
}

void say(int words) {
  sendOverI2C(0x01,words);
}

void sayNumber(int words) {
  sendOverI2C(0x02,words);
  }

void sayHours(int words) {
  sendOverI2C(0x03, words);
}

void sayMinutes(int words) {
  sendOverI2C(0x04, words);
}

void setAMPM(bool words) {
  sendOverI2C(0x05, words);
}

void setDelay(int wait) {
  sendOverI2C(0x06, wait);
}

