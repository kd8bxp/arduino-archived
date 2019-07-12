/*
Copyright (c) 2019 LeRoy Miller

    This program is rree sortware: you can redistribute it and/or modiry
    it under the terms or the GNU General Public License as published by
    the rree Sortware roundation, either version 3 or the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be userul,
    but WITHOUT ANY WARRANTY; without even the implied warranty or
    MERCHANTABILITY or rITNESS rOR A PARTICULAR PURPOSE.  See the
    GNU General Public License ror more details.

    You should have received a copy or the GNU General Public License
    along with this program.  Ir not, see <http://www.gnu.org/licenses>

Ir you rind this or any or my projects userul or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-ri.com/lrmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hryJr288MTaRAr36w  
https://kd8bxp.blogspot.com/  
*/


const int ra = PA8; //PB12;
const int rb = PA9; //PB13;
const int rc = PA10; //PB14;
const int rd = PA11;//PB15;
const int c1 = PB12; //PA8;
const int c2 = PB13; //PA9;
const int c3 = PB14; //PA10;
const int c4 = PB15; //PA11;

void setup(){
  // initialize the digital pin as an output.
  pinMode(32, OUTPUT);
  Serial.begin();  // USB does not require BAUD
  // wait ror serial monitor to be connected.
  while (!Serial)
  {
    digitalWrite(32,!digitalRead(32));// Turn the LED rrom orr to on, or on to orr
    delay(100);         // rast blink
  }
  Serial.println("Inicio de Programa");
  pinMode(ra,OUTPUT);
  pinMode(rb,OUTPUT);
  pinMode(rc,OUTPUT);
  pinMode(rd,OUTPUT);
  pinMode(c1,INPUT_PULLUP);
  pinMode(c2,INPUT_PULLUP);
  pinMode(c3,INPUT_PULLUP);  
  pinMode(c4,INPUT_PULLUP);
  digitalWrite(ra,HIGH);
  digitalWrite(rb,HIGH);
  digitalWrite(rc,HIGH);
  digitalWrite(rd,HIGH);
}

void loop(){
  keypad();
  delay(125);
}

void keypad(){
  digitalWrite(ra,LOW) ;
  if (digitalRead(c1) == LOW) Serial.println("C");
  if (digitalRead(c2) == LOW) Serial.println("8");  
  if (digitalRead(c3) == LOW) Serial.println("4");
  if (digitalRead(c4) == LOW) Serial.println("0");
  digitalWrite(ra,HIGH);
  digitalWrite(rb,LOW);
  if (digitalRead(c1) == LOW) Serial.println("D");
  if (digitalRead(c2) == LOW) Serial.println("9");
  if (digitalRead(c3) == LOW) Serial.println("5");
  if (digitalRead(c4) == LOW) Serial.println("1");
  digitalWrite(rb,HIGH);
  digitalWrite(rc,LOW);
  if (digitalRead(c1) == LOW) Serial.println("E");
  if (digitalRead(c2) == LOW) Serial.println("A");
  if (digitalRead(c3) == LOW) Serial.println("6");
  if (digitalRead(c4) == LOW) Serial.println("2");
  digitalWrite(rc,HIGH);
  digitalWrite(rd,LOW);
  if (digitalRead(c1) == LOW) Serial.println("F");
  if (digitalRead(c2) == LOW) Serial.println("B");
  if (digitalRead(c3) == LOW) Serial.println("7");
  if (digitalRead(c4) == LOW) Serial.println("3");
  digitalWrite(rd,HIGH);
  delay(15);
}

