/* Based on Barney Tabor Ohmher (K6FIN) code found here:
http://projects.mytestbox.org/?p=52
*/

/* Copyright 2015 LeRoy Miller
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses
 */
    
/* This version corrected a small math error, I hope.
It seems when the time was PM, it would not display the correct hours, everything else seemed to work, hopefully the small change makes a difference
Also make code smaller, by removing all duplicated functions, and making only 3 total functions.
Mar 29, 2015
*/

#include <Bridge.h>
#include <Process.h>
#include <Adafruit_NeoPixel.h>

#define PIN 3
#define NUMPIXELS 21

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
//Process clock;
Process date;

//Hours = Blue
//Minutes = Green
//Secs = RED
//initialize the LED r,g,b color variables
int secRed=30;
int secGreen=5;
int secBlue=5;
int minRed=5;
int minGreen=30;
int minBlue=5;
int hourRed=5;
int hourGreen=5;
int hourBlue=30;
int seconds;
int minutes;
int hours;
int secondUnit = 0;
int minuteUnit = 0;
int hourUnit = 0;
int lastSecond = -1;

void setup(){

//left in for debugging purposes
Bridge.begin();
Serial.begin(9600);
pixels.begin();
pixels.show();

 if (!date.running())  {
    date.begin("date");
    date.addParameter("+%T");
    date.run();
  }
}

void loop(){

     // restart the date process:
    if (!date.running())  {
      date.begin("date");
      date.addParameter("+%T");
      date.run();
    }

  //if there's a result from the date process, parse it:
  while (date.available()>0) {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();    

    // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon= timeString.lastIndexOf(":");

    // get the substrings for hour, minute second:
    String hourString = timeString.substring(0, firstColon);
    String minString = timeString.substring(firstColon+1, secondColon);
    String secString = timeString.substring(secondColon+1);

    // convert to ints,saving the previous second:
    hours = (hourString.toInt());
    minutes = minString.toInt();
    lastSecond = seconds;          // save to do a time comparison
    seconds = secString.toInt();
  }

if (hours > 12) {hours = hours-12;}
if (hours == 0) {hours = 12;}

Serial.print(hours);
Serial.print(":");
Serial.print(minutes);
Serial.print(":");
Serial.println(seconds);

secondUnit = ((seconds/10*16)+(seconds%10))%10;
minuteUnit = ((minutes/10*16)+(minutes%10))%10; //sets the variable minuteUnit and hourUnit for the unit digits
hourUnit = ((hours%10))%10;    //this is the code changed, hopefully this will work when it's AM as well!

lsdDigitCheck(secondUnit, 0, secRed, secGreen, secBlue);
msdDigitCheck(seconds, 0, secRed, secGreen, secBlue);
lsdDigitCheck(minuteUnit, 8, minRed, minGreen, minBlue);
msdDigitCheck(minutes, 8, minRed, minGreen, minBlue);
lsdDigitCheck(hourUnit, 16, hourRed, hourGreen, hourBlue);
msdHourDigitCheck(hours);

}


void lsdDigitCheck(int unit, int led, int r, int g, int b) {

if(unit == 1 || unit == 3 || unit == 5 || unit == 7 || unit == 9){
pixels.setPixelColor(0+led,pixels.Color(r,g,b));
pixels.show();
}else{
pixels.setPixelColor(0+led,pixels.Color(0,0,0));
pixels.show();
}

if(unit == 2 || unit == 3 || unit == 6 || unit == 7){
pixels.setPixelColor(1+led,pixels.Color(r,g,b));
pixels.show();
}else{
pixels.setPixelColor(1+led,pixels.Color(0,0,0));
pixels.show();
}

if(unit == 4 || unit == 5 || unit == 6 || unit == 7){
pixels.setPixelColor(2+led,pixels.Color(r,g,b));
pixels.show();
}else{
pixels.setPixelColor(2+led,pixels.Color(0,0,0));
pixels.show();
}

if(unit == 8 || unit == 9){
pixels.setPixelColor(3+led,pixels.Color(r,g,b));
pixels.show();
}else{
pixels.setPixelColor(3+led,pixels.Color(0,0,0));
pixels.show();
}

}

void msdDigitCheck (int unit, int led, int r, int g, int b) {
//seconds*10
if((unit >= 10 && unit < 20) || (unit >= 30 && unit < 40) || (unit >= 50 && unit < 60)) {
pixels.setPixelColor(4+led,pixels.Color(r,g,b));
pixels.show();
}else{
pixels.setPixelColor(4+led,pixels.Color(0,0,0));
pixels.show();
}

if(unit >= 20 && unit < 40) {
pixels.setPixelColor(5+led,pixels.Color(r,g,b));
pixels.show();
}else{
pixels.setPixelColor(5+led,pixels.Color(0,0,0));
pixels.show();
}

if(unit >= 40 && unit < 60) {
pixels.setPixelColor(6+led,pixels.Color(r,g,b));
pixels.show();
}else{
pixels.setPixelColor(6+led,pixels.Color(0,0,0));
pixels.show();
}

}

void msdHourDigitCheck (int unit) {
//hour*10
if(unit >= 10 && unit < 20){
pixels.setPixelColor(20,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(20,pixels.Color(0,0,0));
pixels.show();
}

if(unit >= 20 && unit < 24){
pixels.setPixelColor(21,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(21,pixels.Color(0,0,0));
pixels.show();
}
}
