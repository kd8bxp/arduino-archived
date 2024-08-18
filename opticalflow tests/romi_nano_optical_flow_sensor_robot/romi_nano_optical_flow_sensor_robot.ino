/*
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

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/

#include "SPI.h"
#include "defines.h"

struct MD
{
 byte motion;
 char dx, dy;
 byte squal;
 word shutter;
 byte max_pix;
};

byte frame[ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y];

void setup() {
 pinMode(PIN_SS,OUTPUT);
  pinMode(PIN_MISO,INPUT);
  pinMode(PIN_MOSI,OUTPUT);
  pinMode(PIN_SCK,OUTPUT);
  pinMode(ldir, OUTPUT);
pinMode(rdir, OUTPUT);  
pinMode(lpwm, OUTPUT);
pinMode(rpwm, OUTPUT);
  
    stop();
     
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  
  Serial.begin(9600);

  if(mousecam_init()==-1)
  {
    Serial.println("Mouse cam failed to init");
    while(1);
  }
   delay(1000); 
}

void loop() {
  // if enabled this section produces a bar graph of the surface quality that can be used to focus the camera
  // also drawn is the average pixel value 0-63 and the shutter speed and the motion dx,dy.
  
  int val = mousecam_read_reg(ADNS3080_PIXEL_BURST /*ADNS3080_PIXEL_SUM*/);
  MD md;
//mousecam_read_motion(&md);
int X = 0;
while (X<1) {
  mousecam_read_motion(&md);
 X = X + (int)md.dx;
  int Y = (int)md.dy;
  
  forward(50,50);
  mousecam_write_reg(ADNS3080_MOTION_CLEAR,0x83);
  //delay(10);
  }
  
 // while ((int)md.dx<1) {
 // mousecam_read_motion(&md);
 //int X = (int)md.dx;
 // int Y = (int)md.dy;
  /*if(X > -1) Serial.print("+");
  if(X < 0) Serial.print("-");
  if(abs(X) < 10) Serial.print("0");
  Serial.print(abs(X));
  Serial.print(',');
  if(Y > -1) Serial.print("+");
  if(Y < 0) Serial.print("-");
  if(abs(Y) < 10) Serial.print("0");
  Serial.println(abs(Y));
  */
 // forward(50,50);
  //delay(10);
 // }
  stop();
  //delay(1000);
  //mousecam_read_motion(&md);
  //while ((int)md.dx>-1) {
  //mousecam_read_motion(&md);
 //int X = (int)md.dx;
 // int Y = (int)md.dy;
  /*if(X > -1) Serial.print("+");
  if(X < 0) Serial.print("-");
  if(abs(X) < 10) Serial.print("0");
  Serial.print(abs(X));
  Serial.print(',');
  if(Y > -1) Serial.print("+");
  if(Y < 0) Serial.print("-");
  if(abs(Y) < 10) Serial.print("0");
  Serial.println(abs(Y));
  */
 // back(50,50);
 // delay(10);
 // }
 // stop();
 // delay(1000);
 // mousecam_read_motion(&md);
 // while ((int)md.dy>-1) {
 // mousecam_read_motion(&md);
 //int X = (int)md.dx;
 // int Y = (int)md.dy;
  /*if(X > -1) Serial.print("+");
  if(X < 0) Serial.print("-");
  if(abs(X) < 10) Serial.print("0");
  Serial.print(abs(X));
  Serial.print(',');
  if(Y > -1) Serial.print("+");
  if(Y < 0) Serial.print("-");
  if(abs(Y) < 10) Serial.print("0");
  Serial.println(abs(Y));
  */
 // forward(50,00);
 // delay(10);
 // }
 // stop();
  
 // delay(1000);
 // mousecam_read_motion(&md);
 // while ((int)md.dy<1) {
 // mousecam_read_motion(&md);
 //int X = (int)md.dx;
 // int Y = (int)md.dy;
  /*if(X > -1) Serial.print("+");
  if(X < 0) Serial.print("-");
  if(abs(X) < 10) Serial.print("0");
  Serial.print(abs(X));
  Serial.print(',');
  if(Y > -1) Serial.print("+");
  if(Y < 0) Serial.print("-");
  if(abs(Y) < 10) Serial.print("0");
  Serial.println(abs(Y));
  */
 // forward(0,50);
 // delay(10);
 // }
 // stop();
  while(1);
}
