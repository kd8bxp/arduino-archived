/*
"The Troll" Arduino compatible practical joke machine!
by Patrick Thomas Mitchell
http://www.engineeringshock.com/the-troll-project-page.html

Library version 1.0.2 June 24, 2019 by LeRoy Miller

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

Copyright (c) 2019 LeRoy Miller

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
*/

/* This is still a work in progress, stay tune for updates */

#include "troll.h"
#include "Arduino.h"
#include <SoftwareSPI.h>

#define SCK_PIN A1
#define MISO_PIN A2
#define MOSI_PIN A3
#define CS_PIN A4
#define SELECT *csReg &= ~csBit;
#define DESELECT *csReg |= csBit;
uint8_t csBit, *csReg;  // Chip Select bit and register

troll t(true); // Using the Internal Speaker will rampdown after each use this saves power

void setup() {
  pinMode(CS_PIN,OUTPUT);    // Chip select pins is an output
  digitalWrite(CS_PIN,HIGH); // Set chip select to be HIGH (5v) by default.  The chip on the shield is selected when this line is brought low.
  uint8_t success = SSPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
//  SSPI.setClockDivider(SPI_CLOCK_DIV32); // low frequency SPI
//  SSPI.setBitOrder(MSBFIRST);  // OTP requires MSB first
//  SSPI.setDataMode(SPI_MODE0);  // Use MODE0, as all other modes to not work

SSPI.makeRegMask(CS_PIN, &csReg, &csBit, OUTPUT);
delay(1000);   // One second delay
  digitalWrite(CS_PIN, LOW);
  SSPI.transfer(_RAMPUP);
  SSPI.transfer(0x00);
  digitalWrite(CS_PIN,HIGH);
//t.begin();
}

void loop() {
//Test of Troll Library

//This example shows how easy it is to blink the onboard LED.
//Notice that while the sound is playing the LED is also on. So the sound is blocking
//acting like a delay while playing.
int btn = t.button();
if (btn == LOW) {
  lbt();
digitalWrite(LED, HIGH);
t.sound(RED_ALERT); // This will play the RED ALERT Sound from Star Trek TNG sound 0x38

} else {
digitalWrite(LED, LOW);
//delay(1000);
}
//btn = HIGH;
delay(100);
}

void lbt() {
   digitalWrite(CS_PIN,LOW);
  SSPI.transfer(_PLAY);
  SSPI.transfer(0x06);
  digitalWrite(CS_PIN,HIGH);
  delay(700); 
   digitalWrite(CS_PIN,LOW);
  SSPI.transfer(_PLAY);
  SSPI.transfer(0x8F);
  digitalWrite(CS_PIN,HIGH);
  //delay(700); 
}
