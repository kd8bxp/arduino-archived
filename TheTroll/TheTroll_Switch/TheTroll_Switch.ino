/*
"The Troll" Arduino compatible practical joke machine!
by Patrick Thomas Mitchell
http://www.engineeringshock.com/the-troll-project-page.html

Library version 1.0.3 June 26, 2019 by LeRoy Miller

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

//This is an array of 16 sounds, that can be selected by using the DIP switch
//Each "sound" consists of the location of the sound, and the duration of the sound, so we need a large array
int soundArray[16][16] = {{EVIL_LAUGH1},{EVIL_LAUGH2},{EVIL_LAUGH3},{SCREAM1},{SCREAM2},{ZOMBIE1},{ZOMBIE2},{ZOMBIE3},{WILHELM},{SCREAM_GIRL},{SCREAM_FEMALE},{BALLS},{DOOR},{RATS},{GRUDGE},{BABY}};

void setup() {
t.begin();
}

void loop() {
//Test of Troll Library

//This example builds on the previous button example, now we can select a different sound using the DIP switch
//hitting the SELECT button will activate the new sound.
//We could also use the DIP switch to select a different program to run, or a combination of selecting a different
//sound and a different program.
//Notice that while the sound is playing the LED is also on. So the sound is blocking

int btn = t.button(); //When the button is pressed it goes LOW
if (btn == LOW) {
int readSwitch = t.readSwitch();
digitalWrite(LED, HIGH);
t.sound(soundArray[readSwitch][0],soundArray[readSwitch][readSwitch]); //It looks a little weird, but the sound function needs both the address location of the sound, and the duration of the sound.

} else {
digitalWrite(LED, LOW);
//delay(1000);
}
//btn = HIGH;
delay(100);
}
