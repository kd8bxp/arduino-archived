/*
Copyright (c) 2019 LeRoy Miller

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


const int fa = PB12;
const int fb = PB13;
const int fc = PB14;
const int fd = PB15;
const int c1 = PA8;
const int c2 = PA9;
const int c3 = PA10;

void setup(){
  // initialize the digital pin as an output.
  pinMode(32, OUTPUT);
  Serial.begin();  // USB does not require BAUD
  // wait for serial monitor to be connected.
  while (!Serial)
  {
    digitalWrite(32,!digitalRead(32));// Turn the LED from off to on, or on to off
    delay(100);         // fast blink
  }
  Serial.println("Inicio de Programa");
  pinMode(fa,OUTPUT);
  pinMode(fb,OUTPUT);
  pinMode(fc,OUTPUT);
  pinMode(fd,OUTPUT);
  pinMode(c1,INPUT_PULLUP);
  pinMode(c2,INPUT_PULLUP);
  pinMode(c3,INPUT_PULLUP);  
}

void loop(){
  leerpad();
  delay(100);
}

void leerpad(){
  digitalWrite(fa,LOW) ;
  if (digitalRead(c1) == LOW) Serial.println("1");
  if (digitalRead(c2) == LOW) Serial.println("2");  
  if (digitalRead(c3) == LOW) Serial.println("3");
  digitalWrite(fa,HIGH);
  digitalWrite(fb,LOW);
  if (digitalRead(c1) == LOW) Serial.println("4");
  if (digitalRead(c2) == LOW) Serial.println("5");
  if (digitalRead(c3) == LOW) Serial.println("6");
  digitalWrite(fb,HIGH);
  digitalWrite(fc,LOW);
  if (digitalRead(c1) == LOW) Serial.println("7");
  if (digitalRead(c2) == LOW) Serial.println("8");
  if (digitalRead(c3) == LOW) Serial.println("9");
  digitalWrite(fc,HIGH);
  digitalWrite(fd,LOW);
  if (digitalRead(c1) == LOW) Serial.println("*");
  if (digitalRead(c2) == LOW) Serial.println("0");
  if (digitalRead(c3) == LOW) Serial.println("#");
  digitalWrite(fd,HIGH);
  delay(15);
}

