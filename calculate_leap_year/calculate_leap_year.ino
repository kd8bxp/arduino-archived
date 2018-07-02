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

#include "Word100.h"; //Works with the 100+ Word Shield standard chip

#define CS_PIN 10 //CS pin of your device.

//Word100lbt Word100(CS_PIN); //use Word100lbt for the Little Buddy talker
//Word100ham Word100(CS_PIN); //use with 100+ Word Shield Ham chip
Word100 Word100(CS_PIN); //use with 100+ Word Shield standard chip

bool leapYear;
//int year = 2000; //2000 is a leap year
//int year = 2012; //2012 is a leap year
//int year = 1900; //1900 is not a leap year

int year = 2032;

void setup() {
  Serial.begin(9600);
Word100.begin();

checkLeapYear(year);
Serial.print(year);
Serial.print(" is a ");
Serial.println(leapYear ? "Leap Year" : "Not a Leap Year");
}

void loop() {
  
}

void checkLeapYear(int y) {
  //https://www.wikihow.com/Calculate-Leap-Years
    leapYear = false;
    double temp1 = (float)y/(float)4;
    double temp2 = (float)y/(float)100;
    double temp3 = (float)y/(float)400;
    
    //if (temp1 != (int)temp1) {return(0);}
    if ((temp1 == (int)temp1) && (temp2 != (int)temp2)) {leapYear = true; return(0); }
    if ((temp1 == (int)temp1) && (temp2 == (int)temp2) && (temp3 == (int)temp3)) {leapYear = true; return(0);}
   
    
  }

