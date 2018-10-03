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

#include <Maqueen.h>
Maqueen bot;

//on the makeblock line map
//Black Line returns a Zero (0)
//White returns a one (1)

void setup() {
  Serial.begin(9600);
  bot.begin();
  
}

void loop() {
 //Serial.print("Left: ");
 //Serial.print(patrolLeft());
 //Serial.print(" Right: ");
 //Serial.println(patrolRight());
 //delay(100);
 bot.setSpeed(50);
 if (patrolLeft() == 0 && patrolRight() == 0) {
 bot.forward(); 
 }
 if (patrolLeft() == 1 && patrolRight() == 0) {
  bot.right();
 }
 if (patrolRight() == 1 && patrolLeft() == 0) {
  bot.left();
 }
 if (patrolLeft() == 1 && patrolRight() == 1) {
  bot.spinRight();
 } else if (patrolLeft() == 1 && patrolRight() == 1) {
  bot.spinLeft();
 }
}

int patrolLeft() {
  return (digitalRead(PATROLLEFT));
  }

int patrolRight() {
  return (digitalRead(PATROLRIGHT));
}

