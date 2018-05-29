/*
 * * I2C interface for Little Buddy Talker
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
#include <Wire.h>
#include <SPI.h>
#include <Gamebuino.h>
#include "default.h"; //word Define list
//(old list but works for this demo)

#define REMOTE 0x08 //I2C address of slave device

Gamebuino gb;

int sentence[] = {_one, _two, _three, _four, _five, _six, _seven, _eight, _nine, _ten};
int sentence2[] = {_alert, _alert, _intruder, _in, _zone, _two};

int wait;

/*I2C Registers
 * 0x00 Status
 * 0x01 say
 * 0x02 sayNumber
 * 0x03 sayHours
 * 0x04 sayMinutes
 * 0x05 setAMPM
 * 0x06 setDelay
 */
 
void setup() {
  Wire.begin();
 Serial.begin(9600);
 gb.begin();
 gb.titleScreen(F("Little Buddy Talker Demo"));
 delay(3000); // let Nano boot up and come online
}

#define arr_len( x ) ( sizeof ( x ) / sizeof (*x) )

void loop() {
if(gb.update()) {
  if(gb.buttons.pressed(BTN_A)){
      gb.sound.playOK();
      gb.display.println(F("Start...."));
        wait = getDelay();
//say the first sentence (Count to Ten)  
for (int i = 0; i < arr_len(sentence); i++) {
  say(sentence[i]);
      delay(wait); //default timing
  }

//Change Delay between words
setDelay(450); //you can set it here, but it doesn't really do anything
wait = getDelay(); //but you still have to use a delay in your code
//say the first sentence again with new delay
for (int i = 0; i < arr_len(sentence); i++) {
  say(sentence[i]);
 delay(wait); //so you need to change your delays here also
}

//setDelay doesn't really work, delays need to be controlled by the local sketch
setDelay(700); //set delay back to default of 700
wait = getDelay(); //Will display current set delay (also not working correctly)

//say second sentance
for (int i = 0; i < arr_len(sentence2); i++) {
say(sentence2[i]);
delay(wait); //700 is the default delay between words
}

delay(1000); //wait to move to next demo

//Say a number 293003
sayNumber(293003); //not working says "9" 
//Need to try this with a local sayNumber routine
delay(1000);

//say an hour 
setAMPM(1);
getAMPM(); //See if it is set
sayHours(13); // 1 should say 1 pm
getSpeaking(); //This doesn't work Idea was to check to see if the device was speaking
delay(wait);
//change to 24hr time
setAMPM(0);
getAMPM(); //Should have changed
//say hour agin
sayHours(13); //Should say 13
delay(1000); //delay to next demo
getSpeaking(); //should say 0 doesn't work (Thou does say zero)

setAMPM(1);
sayHours(13); //should say 1
delay(wait); //wait time between words
sayMinutes(43); //minutes look like not working....
delay(1000); //wait time between demo sections

say(_red);
delay(wait);
say(_alert);

Serial.println("End Demo.");

//End Demo and just wait.
    }
    if(gb.buttons.pressed(BTN_B)){
      gb.sound.playCancel();
    }
    if(gb.buttons.pressed(BTN_C)){
      gb.titleScreen(F("Little Buddy Talker Demo"));
    }


//while(1) {}
}
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
  sendOverI2C(0x04, words); //not working same problem as sayNumbers (I think)
}

void setAMPM(bool words) {
  sendOverI2C(0x05, words);
}

void setDelay(int wait) {
  Wire.beginTransmission(REMOTE); //transmit to REMOTE device
  Wire.write(0x06);
  delay(10);
  Wire.write(0xFF);
  delay(100);
  Wire.write((byte*)&wait,2);
  Wire.endTransmission();
  //sendOverI2C(0x06, wait);
}

bool getAMPM() {
  sendOverI2C(0x00, 0x01); //set Status Register to 0x01
  delay(100);
  Wire.requestFrom(REMOTE, 1);
  bool i = Wire.read();
  Serial.print("AMPM setting: ");
  Serial.println(i);
  return i;
}

bool getSpeaking() {
  sendOverI2C(0x00, 0x02); //set Status Register to 0x02
  delay(100);
  Wire.requestFrom(REMOTE, 1);
  bool i = Wire.read();
  Serial.print("IS Speaking? ");
  Serial.println(i);
  return i;
}

int getDelay() {
  //int output = 0;
  sendOverI2C(0x00, 0x03); //set Status Register to 0x03
  delay(100);
  Wire.requestFrom(REMOTE, 2);
int i;
  if (Wire.available() >=2) {
    
    for (int xx = 0; xx < 2; xx++) {
      i += Wire.read() << (xx * 8);
    }
  }
  
  //byte high = Wire.read();
  //delay(100);
  //byte low = Wire.read();
  //int i = (high<<8)+low;
  Serial.print("Delay: ");
  Serial.println(i);
  return i;
}

