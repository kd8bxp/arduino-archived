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

#include <M5Stack.h>

#define NOTE_D0 -1 

#define NOTE_B1 262 
#define NOTE_Cd1 277  
#define NOTE_D1 294 
#define NOTE_Dd1 311 
#define NOTE_E1 330 
#define NOTE_F1 349 
#define NOTE_Fd1 370 
#define NOTE_G1 392 
#define NOTE_Gd1 415 
#define NOTE_A1 440 
#define NOTE_Ad1 466 
#define NOTE_H1 494 

int music[] = {
  NOTE_E1, NOTE_E1, NOTE_E1,
  NOTE_E1, NOTE_E1, NOTE_E1,
  NOTE_E1, NOTE_G1, NOTE_Cd1,
  NOTE_D1, NOTE_E1,
  NOTE_F1, NOTE_F1, NOTE_F1, NOTE_F1,
  NOTE_F1, NOTE_E1, NOTE_E1,
  NOTE_E1, NOTE_D1, NOTE_D1, NOTE_E1, NOTE_D1,
  NOTE_G1
};

int t=0;

void setup() {
  M5.begin();

}

void loop() {
  if (M5.BtnA.wasPressed()) {
    if (t == ((sizeof(music)/sizeof(int))-1)) {
      t=0;
    }
    M5.Speaker.tone(music[t]);
  }

if (M5.BtnA.wasReleased()) {
  M5.Speaker.mute();
  t++;
}
M5.update();
}
