#include <Arduino.h>
#include "Robonii.h"

// set pin numbers:
const int buttonPin = Bumper_Front;     // the number of the pushbutton pin
const int buttonPin2 = Bumper_Rear;     // the number of the pushbutton pin
const int ledPin =  D5_Green;      // the number of the LED pin
const int ledPin2 =  D5_Red;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);     
  
   // initialize the LED pin as an output:
  pinMode(ledPin2, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin2, INPUT_PULLUP);     
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }
  
   buttonState2 = digitalRead(buttonPin2);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState2 == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin2, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin2, LOW); 
  }
  
}