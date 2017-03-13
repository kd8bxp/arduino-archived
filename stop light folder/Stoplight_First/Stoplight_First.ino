/*
  Stop light Simulator
  Created by Frank Hill 3/6/2015
  This program flashes LEDs on the Front Panel
 
  This example code is in the public domain.
 */
 
//Pin assignments
int redfront = 2;
int yellowfront = 3;
int greenfront = 4;
int redrear = 5;
int yellowrear = 6;
int greenrear = 7;
int redleft = 8;
int yellowleft = 9;
int greenleft = 10;
int redright = 11;
int yellowright = 12;
int greenright = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(redfront, OUTPUT);
  pinMode(yellowfront, OUTPUT);
  pinMode(greenfront, OUTPUT);
  pinMode(redrear, OUTPUT);
  pinMode(yellowrear, OUTPUT);
  pinMode(greenrear, OUTPUT);
  pinMode(redleft, OUTPUT);
  pinMode(yellowleft, OUTPUT);
  pinMode(greenleft, OUTPUT);
  pinMode(redright, OUTPUT);
  pinMode(yellowright, OUTPUT);
  pinMode(greenright, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
 
  digitalWrite(greenfront, HIGH);  //green pin on front side goes on
  digitalWrite(redfront, HIGH);  //red pin on front side goes off
  digitalWrite(yellowfront, HIGH);
  delay(15000);              //tells it to not do anything for 15000 milliseconds (15 sec)
  digitalWrite(greenfront, LOW);  //green pin on front side goes on
  digitalWrite(redfront, LOW);  //red pin on front side goes off
  digitalWrite(yellowfront, LOW);
  delay(2000);
  
}