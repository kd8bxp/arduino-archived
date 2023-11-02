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


void setup() {
Serial.begin(9600);
delay(1000);  



}

void loop() {
      Serial.write (0x8A); //Start Command (?)
    Serial.write (0xA8); 
    Serial.write (0x00); //Row
    Serial.write (0x00); //Column
    Serial.write (0x05); //Number of Characters that follow
    Serial.write ( 'H'); 
    Serial.write ( 'e'); 
    Serial.write ( 'l'); 
    Serial.write ( 'l');
    Serial.write ( 'o');
delay(3000);
    Serial.write (0x8A); //Start Command (?)
    Serial.write (0xA8); 
    Serial.write (0x00); //Row
    Serial.write (0x30); //Column
    Serial.write (0x05); //Number of Characters that follow
    Serial.write ( 'W');
    Serial.write('o');
    Serial.write('r');
    Serial.write('l');
    Serial.write('d');
   //Serial.print('Hello World'); 
delay(3000);

}
