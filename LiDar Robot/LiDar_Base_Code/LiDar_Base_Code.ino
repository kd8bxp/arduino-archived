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

/*
 * 10 E-STOP
 * 5 Full Speed  >= 90 Inches
 * 3 Half Speed  < 24 inches
 * 7 Stop and turn <= 8 inches
 * 
 * Distances are checked in this sketch, and the above
 * codes are transmitted to the slave device, which 
 * controls the motors.
 */

#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {
  Serial.begin(9600);
   mySerial.begin(19200);
   
   Wire.begin(); // This is the master device
  inputString.reserve(200);

}

void loop() {
  mySerial.println("F");
  if (mySerial.available() > 0) {
    mySerialEvent();
  }
 if (stringComplete) {
  //Serial.println(inputString);
  inputString.remove(inputString.length()-2);
    if (inputString.indexOf("OK")>0) {
      sendStopCmd();
      //Do nothing 
      } else if (inputString.indexOf("Er")>0) {
      
      sendStopCmd();
             
    } else if (inputString.indexOf("V")>0) {
     
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
     //lcd.print(temp);
     //lcd.print("'C ");
     //lcd.setCursor(10, 0);
     //lcd.print(volts);
     //lcd.print("v");
     //lcd.setCursor(0,1);
     //lcd.print((temp * 9/5) + 32);
     //lcd.print("'F");   
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
      //Serial.print("Strength: ");
      //Serial.print(strength1);
      //Serial.print(" Meter: ");
      //Serial.print(meter);
      //Serial.print("  Inch: ");
      //Serial.println((meter*3.281)*12);
      sendGoCmd((meter*3.281)*12,strength1);

     //Other conversions:
     // cm = meter * 100
     // feet = meter * 3.281
     
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  delay(150);
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

void sendStopCmd() {
  Wire.beginTransmission(8); //transmit to device #8
  Wire.write("C");
  Wire.write(10);
  Wire.endTransmission();
}

void sendGoCmd(int distance, int strength) {
  int cmd;
  if (strength <= 250) {
    if (distance >= 90) {cmd =5; }
    else if (distance <= 24 && distance > 8) {cmd = 3;}
    else if (distance <= 8) {cmd = 7;}
  } else {cmd = 10;}
  Wire.beginTransmission(8);
  Wire.write("C");
  Wire.write(cmd);
  Wire.endTransmission();
}

