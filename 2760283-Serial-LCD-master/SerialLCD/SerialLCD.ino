/***************************************************************************/    
//            Hardware: RadioShack 16x2 Serial LCD
//            Arduino IDE: Arduino-1.0
//            Date:      April 19, 2013
//            Author: RadioShack Corporation 
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, see <http://www.gnu.org/licenses/>
//
/*****************************************************************************/

#include <SoftwareSerial.h>
 
#define txPin 2
 
SoftwareSerial LCD = SoftwareSerial(0, txPin);
// Because the LCD does not return data, define the txPin
const int LCDdelay=10;  // conservative, 2 actually works
 
// wbp: goto with row & column
void lcdPosition(int row, int col) {
  LCD.write(0xFE);   //command flag
  LCD.write((col + row*64 + 128));    //position 
  delay(LCDdelay);
}
void clearLCD(){
  LCD.write(0xFE);   //command flag
  LCD.write(0x01);   //clear command.
  delay(LCDdelay);
}
void backlightOn() {  //turns on the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(157);    //light level. 157=MAX, 128=OFF
  delay(LCDdelay);
}
void backlightOff(){  //turns off the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(128);     //light level for off.
   delay(LCDdelay);
}
void serCommand(){   //a general function to call the command flag for issuing all other commands   
  LCD.write(0xFE);
}
 
void setup()
{
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  backlightOn() ;
  clearLCD();
  lcdPosition(0,0);
  LCD.print("RadioShack says Hi!");
}
 
void loop()
{
}
