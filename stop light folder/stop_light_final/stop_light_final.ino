/*
  
  Created by Frank Hill 3/6/2015
  Stop light Simulator
  
  This program reads the toggle switch which is connected to PIN 4. If the switch is ON
  or left, the normal Stop Light program will run. If the switch is off, the flashing test program
  will run.
 
  This example code is in the public domain.
 */
 

int redfront = 2;
int yellowfront = 3;
int greenfront = 14; // Changed from previous programs
int redrear = 5;
int yellowrear = 6;
int greenrear = 7;
int redleft = 8;
int yellowleft = 9;
int greenleft = 10;
int redright = 11;
int yellowright = 12;
int greenright = 13;

// Added Code
int SwitchState = 0; //Toggle Switch
int ExternalSW = 4; //Connected to pin 4 with a pullup resistor

// the setup routine runs once when you press reset:

void setup() {                
  // initialize the digital pin as an output.
  pinMode(ExternalSW, INPUT); // Added Code
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
  
  // Check Switch State
  
   if(SwitchState == HIGH)            // Turn Yellow LED on or off based on Switch State
    {
    void Normal_Stop();
    }
  else
  {
    void Test_Stop();
  }
  
}

void Normal_Stop() {
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

void Test_Stop() {
  // Flash All Lights
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