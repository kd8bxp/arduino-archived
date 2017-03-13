/*
 Created by Frank Hill 3/6/2016 

  Stop light Simulator
  Turns on green, red, and yellow leds on in a flashing pattern repeatedly.
 
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
  // Turn on front lights
  digitalWrite(greenfront, HIGH);  //green pin on front side goes on
  digitalWrite(redfront, HIGH);  //red pin on front side goes off
  digitalWrite(yellowfront, HIGH);
  digitalWrite(greenrear, HIGH);  //green pin on front side goes on
  digitalWrite(redrear, HIGH);  //red pin on front side goes off
  digitalWrite(yellowrear, HIGH);
  digitalWrite(greenright, HIGH);  //green pin on front side goes on
  digitalWrite(redright, HIGH);  //red pin on front side goes off
  digitalWrite(yellowright, HIGH);
  digitalWrite(greenleft, HIGH);  //green pin on front side goes on
  digitalWrite(redleft, HIGH);  //red pin on front side goes off
  digitalWrite(yellowleft, HIGH);
  delay(2000);
  // Turn Off front lights
  digitalWrite(greenfront, LOW);  //green pin on front side goes on
  digitalWrite(redfront, LOW);  //red pin on front side goes off
  digitalWrite(yellowfront, LOW);
  digitalWrite(greenrear, LOW);  //green pin on front side goes on
  digitalWrite(redrear, LOW);  //red pin on front side goes off
  digitalWrite(yellowrear, LOW);
  digitalWrite(greenright, LOW);  //green pin on front side goes on
  digitalWrite(redright, LOW);  //red pin on front side goes off
  digitalWrite(yellowright, LOW);
  digitalWrite(greenleft, LOW);  //green pin on front side goes on
  digitalWrite(redleft, LOW);  //red pin on front side goes off
  digitalWrite(yellowleft, LOW);
  delay(2000);
}