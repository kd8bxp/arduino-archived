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

#define ldir 4 
#define lpwm 6 //10 
#define rdir 7 //5 
#define rpwm 5 //9
//Use L9110 motor driver

#define armPinA 2 
#define armPinB 3 

void setup() {
   Serial.begin(9600);
  Serial.print("Starting...\n");
pinMode(ldir, OUTPUT);
pinMode(rdir, OUTPUT);  
pinMode(lpwm, OUTPUT);
pinMode(rpwm, OUTPUT);
pinMode(armPinA, OUTPUT);
pinMode(armPinB, OUTPUT);
stop();
open(); 
armStop();
    delay(1000); 
}

void loop() {
  
forward(50,50); //left, right PWM values
delay(250);
stop();
close();
armStop();
delay(1000);
back(75,75);
delay(1000);
stop();
back(50,150);
delay(250);
stop();
open();
armStop();
back(75,75);
delay(1000);
stop();
forward(150,50);
delay(250);
stop();
while(1);
}

void armStop() {
  //analogWrite(armPinB, 0);
  digitalWrite(armPinB, 0);
  digitalWrite(armPinA, 0);
  
}

void close() {
  //analogWrite(lpwm, 50);
  digitalWrite(armPinB, HIGH);
  digitalWrite(armPinA, LOW);
  delay(2500);
}

void open() {
  //analogWrite(lpwm, 25);
  digitalWrite(armPinB, LOW);
  digitalWrite(armPinA, HIGH);
  delay(2500);
}

void stop() {
  analogWrite(lpwm, 0);
  analogWrite(rpwm, 0);
  digitalWrite(ldir, 0);
  digitalWrite(rdir, 0);
}

void back(int lspd, int rspd) {
  //because our robot switch the front for the back
  //we need to switch the direction of the motors in software
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, HIGH);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, HIGH);
}

void forward(int lspd, int rspd) {
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, LOW);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, LOW);
}


