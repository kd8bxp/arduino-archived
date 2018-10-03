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

#include <Adafruit_Microbit.h>

Adafruit_Microbit_Matrix microbit;

const int DOWN = 13;
const int LEFT = 14;
const int UP = 8;
const int RIGHT = 15;
const int X = 1;
const int Y = 2;
const int LED = 16;
const int BUZZER = 0;
const int VIBRATE = 12;
const int A = 5;
const int B = 11;

void setup() {
Serial.begin(9600);
microbit.begin();
pinMode (UP, INPUT);
pinMode (DOWN, INPUT);
pinMode (LEFT, INPUT);
pinMode (RIGHT,INPUT);
pinMode (A, INPUT);
pinMode (B, INPUT);
pinMode (X, INPUT);  
pinMode (Y, INPUT);
pinMode (LED, OUTPUT);
pinMode (BUZZER, OUTPUT);
pinMode (VIBRATE, OUTPUT);
//digitalWrite(IR, HIGH);
analogWrite(BUZZER, 2055);
digitalWrite(VIBRATE, HIGH);
delay(100);
analogWrite(BUZZER, 0);
digitalWrite(VIBRATE, LOW);

}

void loop() {
  digitalWrite(LED, HIGH);
  analogWrite(BUZZER, 2055);
  digitalWrite(VIBRATE, HIGH);
if (digitalRead(UP) == 0) {microbit.print("U");}
if (digitalRead(DOWN) == 0) {microbit.print("D");}
if (digitalRead(LEFT) == 0) {microbit.print("L");}
if (digitalRead(RIGHT) == 0) {microbit.print("R");}
if (digitalRead(X) == 0) {microbit.print("X"); }
if (digitalRead(Y) == 0) {microbit.print("Y"); }
if (digitalRead(A) == 0) {microbit.print("A"); }
if (digitalRead(B) == 0) {microbit.print("B"); }
delay(250);
digitalWrite(LED, LOW);
analogWrite(BUZZER, 0);
digitalWrite(VIBRATE, LOW);
delay(250);
}
