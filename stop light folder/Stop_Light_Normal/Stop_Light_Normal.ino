/*
  Stop light Simulator
  Created by Frank Hill on 3/6/2015
  Runs stop light sequence in accordance with normal stop light operation.
 
  This example code is in the public domain.
 */
 

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
  digitalWrite(greenright, LOW); //green pin on right side goes off
  digitalWrite(greenleft, LOW);  //green pin on left side goes off
  digitalWrite(greenfront, HIGH);  //green pin on front side goes on
  digitalWrite(greenrear, HIGH);  //green pin on rear side goes on
  digitalWrite(redleft, HIGH);  //red pin on left side goes on
  digitalWrite(redright, HIGH);  //red pin on right side goes on
  digitalWrite(redrear, LOW);  //red pin on rear side goes off
  digitalWrite(redfront, LOW);  //red pin on front side goes off
  digitalWrite(yellowrear, LOW);  //etc.
  digitalWrite(yellowfront, LOW);
  digitalWrite(yellowleft, LOW);
  digitalWrite(yellowright, LOW);  
  delay(15000);              //tells it to not do anything for 15000 milliseconds (15 sec)
  digitalWrite(yellowfront, HIGH);  //yellow pins on front and rear go on 2000 milliseconds (2 sec)
  digitalWrite(yellowrear, HIGH);
  delay(2000);
  digitalWrite(greenrear, LOW);  //the opposite of the front portion of the loop
  digitalWrite(greenfront, LOW);
  digitalWrite(greenleft, HIGH);  
  digitalWrite(greenright, HIGH);
  digitalWrite(redrear, HIGH);
  digitalWrite(redfront, HIGH);
  digitalWrite(redleft, LOW);
  digitalWrite(redright, LOW);
  digitalWrite(yellowleft, LOW);
  digitalWrite(yellowright, LOW);
  digitalWrite(yellowfront, LOW);
  digitalWrite(yellowrear, LOW);  
  delay(15000);              
  digitalWrite(yellowleft, HIGH);  
  digitalWrite(yellowright, HIGH);
  delay(2000);
  
}