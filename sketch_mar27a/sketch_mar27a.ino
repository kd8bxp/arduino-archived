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

#define _one 0x10,1
#define _two 0x11,2
#define _three 0x12,3
#define _four 0x13,1
#define _five 0x14,2


void setup() {
  Serial.begin(9600);
 say(_one);
 say(_two);
 say(_three);
 say(_four);
 say(_five);
}

void loop() {
  

}

void say(int number, int cs) {
  //int cs = word[1];
  //int number = word[0];
  Serial.print("CS: ");
  Serial.print(cs);
  Serial.print(" Hex: ");
  Serial.println(number,HEX);
}

