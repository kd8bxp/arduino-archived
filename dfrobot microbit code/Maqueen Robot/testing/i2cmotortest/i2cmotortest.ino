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

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  delay(100);
  stop();
  //delay(500);
}

byte x = 0;

void loop() {
/*  Wire.beginTransmission(0x10); // transmit to device #8
  Wire.write((byte)0x02);        //Motor Left (0x00) or Right (0x02)
  Wire.write((byte)0x00);       //CW (0x00) or CCW (0x01)
  Wire.write((byte)0x50);        //Speed
  Wire.endTransmission();    // stop transmitting
while(1);
*/

forward();
  
  delay(500);
  stop();
  delay(500);
  backward();
  delay(500);
  stop();
  delay(500);
  spinleft();
  delay(500);
  stop();
  delay(500);
  spinright();
  delay(500);
  stop();
  left();
  delay(500);
  stop();
  delay(500);
  right();
  delay(500);
  stop();
  while(1);
}

void stop() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
}

void forward() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void backward() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x01);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x01);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void spinleft() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x01);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void spinright() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x01);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void left() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
}

void right() {
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write((byte)0x50); //speed
  Wire.endTransmission();
  Wire.beginTransmission(0x10);
  Wire.write((byte)0x02);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00); //speed
  Wire.endTransmission();
}




