/*
"The Troll" Arduino compatible practical joke machine!
by Patrick Thomas Mitchell
http://www.engineeringshock.com/the-troll-project-page.html

Library version 1.0.4 June 26, 2019 by LeRoy Miller

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

troll t(true); // Using the Internal Speaker will rampdown after each use this saves power

//This is an array of 4 sounds, that can be selected by using the DIP switch using sw3 and sw4.
//Each "sound" consists of the location of the sound, and the duration of the sound, so we need a large array
int soundArray[4][4] = {{EVIL_LAUGH1},{SCREAM1},{ZOMBIE1},{WILHELM}};

//Default is for sound zero, and program zero
int Sound = 0; //hold the vaule for the sound selected above
int Prg = 1; //hold the vaule for the program selected

unsigned long previousHauntingMillis = 0;
unsigned long hauntingInterval = 10; //1st interval is set, each additional interval will be randomized
unsigned long previousBuggerMillis = 0;
unsigned long buggerInterval = 10;
int randomSound; //will pick a random sound from the whole sound library

//SW1 and SW2 control which program is going to run. 
//program 00 - lightsout - turn the lights out, when the lights comeon a sound will trigger
//program 01 - haunting - semi-random timing of sound
//program 02 - bugger - like haunting, but with semi-random timings and random sounds from the soundArray
//program 03 - unknown (maybe a test program)

void setup() {
t.begin();
randomSeed(LDR); //We are going to use the LDR on analog zero to set a random seed value.
readSwitches(); //get the power on state of the DIP switch, just because we may not have them set for the "default" zero/zero
}

void loop() {
//Test of Troll Library

//This example builds on the previous button example, now we can select a different sound using the DIP switch
//hitting the SELECT button will read the new program and sound.


int btn = t.button(); //When the button is pressed it goes LOW
if (btn == LOW) {
  readSwitches();

//t.sound(soundArray[readSwitch][0],soundArray[readSwitch][readSwitch]); //It looks a little weird, but the sound function needs both the address location of the sound, and the duration of the sound.

} 

switch (Prg) {
  case 0:
  lightsout();
  break;
  case 1:
  haunting();
  break;
  case 2:
  bugger();
  break;
  case 3:
  delay(1000);
  test(); 
  break;
  default:
  break;
}
delay(100);
}

void readSwitches() {
    //we are going to read the switches directly because we need to split the values up
int sw4 = digitalRead(SW4); //The switches are defined in the library and can be used in your programs 
int sw3 = digitalRead(SW3);
int sw2 = digitalRead(SW2);
int sw1 = digitalRead(SW1);
Sound = 0;
Prg = 0;

//In the previous example we used a 4 bit version of BCD, giving us a total of 16 possiable uses (0 to 15), in this example
//we are going to use a short version of BCD, where SW4 and SW2 are the ones bit, and SW3 and SW1 are the twos bit
//this should give us 4 sounds, and 4 programs to use - all switches off zero, one bit set - program/sound 1, twos bit set
// program/sound 2, both one and twos bit set - program/sound 3.

//set our sound
if (sw4 == LOW) { Sound = Sound + 1; }
if (sw3 == LOW) { Sound = Sound + 2; }

//set our program
if (sw2 == LOW) { Prg = Prg + 1; }
if (sw1 == LOW) { Prg = Prg + 2; }

}

void lightsout()
{
  
  if(analogRead(LDR) > 900) 
  {    
    for (int i = 0; i <= 5; i++) {
      digitalWrite(LED,HIGH);
      delay(100);
      digitalWrite(LED,LOW);
      delay(100);
    }
  //delay(2000);
  while (analogRead(LDR) < 900) {
  delay(100);
  }
  delay(500);
   while(analogRead(LDR) > 900) { digitalWrite(LED,LOW); };
    t.sound(soundArray[Sound][0],soundArray[Sound][Sound]); //It looks a little weird, but the sound function needs both the address location of the sound, and the duration of the sound.
  }
}


void haunting() {
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousHauntingMillis >= (hauntingInterval*1000)) {
    digitalWrite(LED, HIGH);
    previousHauntingMillis = currentMillis;
    t.sound(soundArray[Sound][0],soundArray[Sound][Sound]); //It looks a little weird, but the sound function needs both the address location of the sound, and the duration of the sound.
    hauntingInterval = random(10, 61); //should pick a new semi-randomized interval between 10 and 60 seconds, this can be adjusted.
  }
  digitalWrite(LED,LOW);
}

void bugger() {
//bugger mode picks a semi-random sound, as well as a semi-random time interval
randomSound = random(0, 58);
 
  unsigned long currentMillis = millis();
  if (currentMillis - previousBuggerMillis >= (buggerInterval*1000)) {
    digitalWrite(LED, HIGH);
    previousBuggerMillis = currentMillis;
    t.sound(randomSound,timings[randomSound]); //It looks a little weird, but the sound function needs both the address location of the sound, and the duration of the sound.
    buggerInterval = random(0, 61); //should pick a new semi-randomized interval between 0 and 60 seconds, this can be adjusted.
  }
  digitalWrite(LED,LOW);

}

void test() {

}
