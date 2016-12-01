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
    
/* UPdated code can be found here
https://codebender.cc/sketch:99119
found a small math error, and corrected it I hope.
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
hourUnit = ((hours/10*16)+(hours%10))%10;

if(secondUnit == 1 || secondUnit == 3 || secondUnit == 5 || secondUnit == 7 || secondUnit == 9){
pixels.setPixelColor(0,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(0,pixels.Color(0,0,0));
pixels.show();
}

if(secondUnit == 2 || secondUnit == 3 || secondUnit == 6 || secondUnit == 7){
pixels.setPixelColor(1,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(1,pixels.Color(0,0,0));
pixels.show();
}

if(secondUnit == 4 || secondUnit == 5 || secondUnit == 6 || secondUnit == 7){
pixels.setPixelColor(2,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(2,pixels.Color(0,0,0));
pixels.show();
}

if(secondUnit == 8 || secondUnit == 9){
pixels.setPixelColor(3,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(3,pixels.Color(0,0,0));
pixels.show();
}

//seconds*10
if((seconds >= 10 && seconds < 20) || (seconds >= 30 && seconds < 40) || (seconds >= 50 && seconds < 60)) {
pixels.setPixelColor(4,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(4,pixels.Color(0,0,0));
pixels.show();
}

if(seconds >= 20 && seconds < 40) {
pixels.setPixelColor(5,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(5,pixels.Color(0,0,0));
pixels.show();
}

if(seconds >= 40 && seconds < 60) {
pixels.setPixelColor(6,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(6,pixels.Color(0,0,0));
pixels.show();
}

//minutes
if(minuteUnit == 1 || minuteUnit == 3 || minuteUnit == 5 || minuteUnit == 7 || minuteUnit == 9){
pixels.setPixelColor(8,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(8,pixels.Color(0,0,0));
pixels.show();
}

if(minuteUnit == 2 || minuteUnit == 3 || minuteUnit == 6 || minuteUnit == 7){
pixels.setPixelColor(9,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(9,pixels.Color(0,0,0));
pixels.show();
}

if(minuteUnit == 4 || minuteUnit == 5 || minuteUnit == 6 || minuteUnit == 7){
pixels.setPixelColor(10,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(10,pixels.Color(0,0,0));
pixels.show();
}

if(minuteUnit == 8 || minuteUnit == 9){
pixels.setPixelColor(11,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(11,pixels.Color(0,0,0));
pixels.show();
}

//minutes*10
if((minutes >= 10 && minutes < 20) || (minutes >= 30 && minutes < 40) || (minutes >= 50 && minutes < 60)) {
pixels.setPixelColor(12,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(12,pixels.Color(0,0,0));
pixels.show();
}

if(minutes >= 20 && minutes < 40) {
pixels.setPixelColor(13,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(13,pixels.Color(0,0,0));
pixels.show();
}

if(minutes >= 40 && minutes < 60) {
pixels.setPixelColor(14,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(14,pixels.Color(0,0,0));
pixels.show();
}

//hours
if(hourUnit == 1 || hourUnit == 3 || hourUnit == 5 || hourUnit == 7 || hourUnit == 9){
pixels.setPixelColor(16,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(16,pixels.Color(0,0,0));
pixels.show();
}

if(hourUnit == 2 || hourUnit == 3 || hourUnit == 6 || hourUnit == 7){
pixels.setPixelColor(17,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(17,pixels.Color(0,0,0));
pixels.show();
}

if(hourUnit == 4 || hourUnit == 5 || hourUnit == 6 || hourUnit == 7){
pixels.setPixelColor(18,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(18,pixels.Color(0,0,0));
pixels.show();
}

if(hourUnit == 8 || hourUnit == 9){
pixels.setPixelColor(19,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(19,pixels.Color(0,0,0));
pixels.show();
}

//hour*10
if(hours >= 10 && hours < 20){
pixels.setPixelColor(20,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(20,pixels.Color(0,0,0));
pixels.show();
}

if(hours >= 20 && hours < 24){
pixels.setPixelColor(21,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(21,pixels.Color(0,0,0));
pixels.show();
}
}