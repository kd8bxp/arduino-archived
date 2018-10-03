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

#include <Maqueen.h>

//Define our IR Codes
//for this experiment we are only going to use 5 buttons on the remote
#define UP 3 || 64 || 15
#define LEFT 7 || 127
#define RIGHT 9 || 39
#define DOWN 25 || 103
#define SETUP 87 || 21

Maqueen bot;

//NewPing.h and Adafruit_Microbit.h are inlcuded the library header
//but still need to be invoked here.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Adafruit_Microbit_Matrix microbit;

//setup variables
int IRPIN= 16;
unsigned long lengthHeader;
unsigned long bit;
int byteValue;
int speed = 50; //50% of full speed

void setup() {
   Serial.begin(9600);
  bot.begin();
  microbit.begin();
  microbit.clear();
  microbit.show(smile_bmp);
  pinMode(IRPIN, INPUT);

}

void loop() {
readIR();

if (byteValue == 0) {  bot.stop(); }
if (byteValue == UP) { bot.setSpeed(speed);
  bot.forward();
}
else if (byteValue == DOWN) {
  bot.setSpeed(speed);
  bot.backward();
}
else if (byteValue == LEFT) {
  bot.setSpeed(speed);
  bot.spinLeft();
}
else if (byteValue == RIGHT) {
  bot.setSpeed(speed);
  bot.spinRight();
}
else if (byteValue == SETUP) {
  bot.stop();
}
// byteValue = 0; 
}

void readIR() {
  //look for a header pulse from the IR Receiver
lengthHeader = pulseIn(IRPIN, LOW);
if(lengthHeader > 5000)
{
//step through each of the 32 bits that streams from the remote
byteValue = 0;
for(int i = 1; i <= 32; i++)
{
bit = pulseIn(IRPIN, HIGH);

//read the 8 bits that are specifically the key code
//use bitwise operations to convert binary to decimal
if (i > 16 && i <= 24)
if(bit > 1000)
byteValue = byteValue + (1 << (i - 17)); 
}

//send the key code to the processing.org program
Serial.println(byteValue);
//Serial.flush();
}

}

