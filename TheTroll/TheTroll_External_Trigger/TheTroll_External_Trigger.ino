/*
"The Troll" Arduino compatible practical joke machine!
by Patrick Thomas Mitchell
http://www.engineeringshock.com/the-troll-project-page.html

Library version 1.0.6 July 2, 2019 by LeRoy Miller

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

/* Sensors active with HIGH output:
 *      PIR
 *      REED SWITCH
 *      touch sensor
 *      
 * Sensors active with LOW output:    
 *      virbration switch
 *      limit switch
 */

#include "troll.h"

troll t(true); // Using the Internal Speaker will rampdown after each use this saves power

unsigned long previousBuggerMillis = 0;
unsigned long buggerInterval = 10;
int randomSound; //will pick a random sound from the whole sound library
int trigger; //This will be set to either high or low from DIP switch 4.  So if your external trigger normal output is always a HIGH, then we need to look for a LOW, and if it's a normal output is LOW then we look for a HIGH
unsigned long previousResetMillis = 0;
unsigned long resetTime = 0; // time in minutes. Resets after min. once triggered bugger mode will be activated until reset. Can be set with DIP switches 3, 2, 1 - up to 7 mins.
bool buggerActive = false; 

void setup() {
t.begin();
randomSeed(LDR); //We are going to use the LDR on analog zero to set a random seed value.

delay(500);
}

void loop() {
  readSwitches(); //change the dip switches on the fly
 if (t.externalTrigger() == trigger) {
  buggerActive = true;
  previousResetMillis = millis();
 }
   
  while (buggerActive == true) {
    digitalWrite(LED,HIGH);
    delay(200);
    bugger();
    digitalWrite(LED,LOW);
    delay(200);
  }
  delay(10);
 
} 

void readSwitches() {
    //we are going to read the switches directly because we need to split the values up
    //for this example we only need to use one of the DIP switches - I choose SW4

if (digitalRead(SW4) == LOW) { trigger = HIGH; } else { trigger = LOW; }

//The switches are defined in the library and can be used in your programs 

// We are going to use SW3, SW2, SW1 to set the reset timer.  SW3 will be the Ones Bit, SW2 the Twos Bit, and SW1 the fours bit.
// for a total of 7 mins.
resetTime = 0;
if (digitalRead(SW3) == LOW) { resetTime = resetTime + 1; }
if (digitalRead(SW2) == LOW) { resetTime = resetTime + 2; }
if (digitalRead(SW1) == LOW) { resetTime = resetTime + 4; }

}

void bugger() {
  
//bugger mode picks a semi-random sound, as well as a semi-random time interval
randomSound = random(0, 58);
 
  unsigned long currentMillis = millis();

  if ((currentMillis - previousBuggerMillis >= (buggerInterval*1000)) || (resetTime == 0)) {
    digitalWrite(LED, HIGH);
    previousBuggerMillis = currentMillis;
    t.sound(randomSound,timings[randomSound]); //It looks a little weird, but the sound function needs both the address location of the sound, and the duration of the sound.
    buggerInterval = random(0, 61); //should pick a new semi-randomized interval between 0 and 60 seconds, this can be adjusted.
  }
  digitalWrite(LED,LOW);
  
if (currentMillis - previousResetMillis >= (resetTime * 60 * 1000)) {
  previousResetMillis = currentMillis;
  buggerActive = false;
  
    }
 
}
