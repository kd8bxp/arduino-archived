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

/* Updated Sep. 2, 2024. This is kind of a failed project
I updated the turning a little, and added a check for a 0 distance,
and a distance greater then 150, as that seems like an error. But it
didn't change much about how this works.  When it gets too close to
something or if it hits something at an angle, it has a hard time
figuring out the distance. 
Project will be archived, I may revisit it sometime in the future
*/

/*
 * 10 E-STOP
 * 5 Full Speed  >= 25 Inches
 * 3 Half Speed  between 24 and 8 inches
 * 7 Stop and turn <= 8 inches
 * 
 */

#include <SoftwareSerial.h>
#include "src/Adafruit_NeoPixel/Adafruit_NeoPixel.h"

#define left1 12
#define left2 11 //pwm
#define right1 10
#define right2 9 //pwm
#define THRESHOLD 800 //signal strength Threshold good reading will be less than or equal to this number
#define FULLSPDDISTANCE 25 //Anything greater than this number in inches will be at FULL speed (topSpeed below)
#define HALFSPDDISTANCE 24 //Should be one less than above
#define TURNDISTANCE 10 //The distance in inches that you want the robot to stop and turn
#define topSpeed 175
#define halfSpeed 100
#define NEOPIXELPIN 13
#define RED strip.Color(155,0,0)
#define GREEN strip.Color(0,155,0)
#define YELLOW strip.Color(140,130,0)
#define OFF strip.Color(0,0,0)

SoftwareSerial mySerial(3, 2); // RX, TX
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
byte pixelState = B00000000; //sets neopixels to on or off
int cmd;

void setup() {
  Serial.begin(115200);
  mySerial.begin(19200);
  inputString.reserve(200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(50);
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  stop(); //make sure motors are stopped
}

void loop() {
  mySerial.println("F");
    if (mySerial.available() > 0) {
      mySerialEvent();
    }
    if (stringComplete) {
      strComplete();
    }
  cmdMove();
  delay(50);
}

void mySerialEvent() {
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void strComplete() {
  //Serial.println(inputString);
  inputString.remove(inputString.length()-2);
    if (inputString.indexOf("OK")>0 || inputString.indexOf("Er")>0) {
      //Serial.println("Error or OK:");
      cmd=10; //Stop Command
      setPixels(0); //LED Color Bar
        }
    else if (inputString.indexOf("V")>0) {
     //get voltage     
     String volt = inputString;
     volt.remove(0,volt.indexOf("V")-3);
     volt.remove(volt.indexOf("V"));
     char arr[volt.length()];
     volt.toCharArray(arr, volt.length()+1);
     float volts = atof(arr);
     
     //get tempature in degrees C
     inputString.remove(0,3);
     inputString.remove(inputString.indexOf("V")-6);
     char arr1[inputString.length()];
     inputString.toCharArray(arr1, inputString.length()+1);
     float temp = atof(arr1);
     //temp is in degrees C
     //(temp*9/5)+32 for degrees F
     } else {
      
      //Get signal strength for comparison.
      String strength = inputString;
      strength.remove(0,strength.indexOf(",")+1);
      //Serial.println(strength);
      char arr1[strength.length()];
      strength.toCharArray(arr1, strength.length()+1);
      int strength1 = atof(arr1);    
      //Get distance in meters
      inputString.remove(inputString.indexOf("m"));
      inputString.remove(0,2);
      char arr[inputString.length()];
      inputString.toCharArray(arr, inputString.length()+1);
      float meter = atof(arr);

      //convert distance to inches
      Serial.print("Strength: ");
      Serial.print(strength1);
      Serial.print(" Meter: ");
      Serial.print(meter);
      Serial.print("  Inch: ");
      Serial.println((meter*3.281)*12);
      goCmd((meter*3.281)*12,strength1);

     //Other conversions:
     // cm = meter * 100
     // feet = meter * 3.281
     
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  
}

void goCmd(int distance, int strength) {
  
  if (strength <= THRESHOLD) {
    setPixels(distance); //LED Color Bar
    if (distance == 0 || distance >= 150) { cmd=7; } 
    else if (distance >= FULLSPDDISTANCE && distance <= 150 ) {cmd = 5; }
    else if (distance <= HALFSPDDISTANCE && distance > TURNDISTANCE) {cmd = 3;}
    else if (distance <= TURNDISTANCE) {cmd = 7;}
  } else {cmd = 10;}
  
}

void cmdMove() {
  switch(cmd) {
    case 3:
    //Serial.println("HalfSpeed");
    ccw(halfSpeed,halfSpeed);
    break;
    case 5:
    //Serial.println("TopSpeed");
    ccw(topSpeed,topSpeed);
    break;
    case 7:
    //Serial.println("Turn");
    stop();
    //cw(60,200);
    //Spin in place (?)
    digitalWrite(right1, 1);
    analogWrite(right2, 0); //255 - 60);
    digitalWrite(left1, 0);
    analogWrite(left2, 255);
    delay(50); //long delay is bad
    break;
    case 10:
    //Serial.println("E-Stop");
    stop();
    break;    
    default:
    //Serial.println("Default Stop");
    stop();
    break;
     }
     //cmd = 0;
}

void cw(int lft, int rgt) { 
  //This causes the car to move backward
  digitalWrite(left1, 1);
  digitalWrite(right1, 1);
  analogWrite(left2, map(lft,0,255,255,0));
  analogWrite(right2,map(rgt,0,255,255,0));  
}

void ccw(int lft, int rgt) {
  //This causes the car to move forward
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  analogWrite(left2, lft);
  analogWrite(right2, rgt);
}

void stop() {
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  analogWrite(left2, 0);
  analogWrite(right2, 0);
}

void setPixels(int i) {  
  
  if (i >= 25) { pixelState = B11111111; }
  if ( i <= 24 && i > 20 ) { pixelState = B11111110; }
  if ( i <= 20 && i > 17 ) { pixelState = B11111100; }
  if ( i<=17 && i> 14) { pixelState = B11111000; }
  if ( i<=14 && i> 10) { pixelState = B11110000; }
  if (i<=10 && i>7) { pixelState = B11100000; }
  if (i<=7 && i>4) { pixelState = B11000000; }
  if (i<=4 && i>1) { pixelState = B10000000; }
  if (i == 0) { pixelState = B00000000; }
  
   strip.setPixelColor(7, bitRead(pixelState,7) ? RED : OFF);
   strip.setPixelColor(6, bitRead(pixelState,6) ? RED : OFF);
   strip.setPixelColor(5, bitRead(pixelState,5) ? RED : OFF);
   strip.setPixelColor(4, bitRead(pixelState,4) ? YELLOW : OFF);
   strip.setPixelColor(3, bitRead(pixelState,3) ? YELLOW : OFF);
   strip.setPixelColor(2, bitRead(pixelState,2) ? YELLOW : OFF);
   strip.setPixelColor(1, bitRead(pixelState,1) ? GREEN : OFF);
   strip.setPixelColor(0, bitRead(pixelState,0) ? GREEN : OFF);
strip.show();
     
}
