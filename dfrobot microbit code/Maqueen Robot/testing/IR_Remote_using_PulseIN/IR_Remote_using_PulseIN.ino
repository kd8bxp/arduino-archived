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

/* This code found here: http://pscmpf.blogspot.com/2010/01/arduino-and-ir-receiver.html
 *  This mostly works and give different numbers for each
 *  button, and is semi-repeatable. Sometimes the wrong numbers
 *  do come up and sometimes the same number for a different button
 *  This was tried with a couple of different remotes, and 
 *  different results for each were seen.
  */
  
//setup variables
int pinData= 16;
unsigned long lengthHeader;
unsigned long bit;
int byteValue;

void setup()
{
pinMode(pinData, INPUT);
Serial.begin(9600);
}

void loop()
{
//look for a header pulse from the IR Receiver
lengthHeader = pulseIn(pinData, LOW);
if(lengthHeader > 5000)
{
//step through each of the 32 bits that streams from the remote
byteValue = 0;
for(int i = 1; i <= 32; i++)
{
bit = pulseIn(pinData, HIGH);

//read the 8 bits that are specifically the key code
//use bitwise operations to convert binary to decimal
if (i > 16 && i <= 24)
if(bit > 1000)
byteValue = byteValue + (1 << (i - 17)); 
}

//send the key code to the processing.org program
Serial.println(byteValue);
Serial.flush();
}

delay(100);
}
