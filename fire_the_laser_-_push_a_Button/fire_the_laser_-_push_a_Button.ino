/*
 Fire the laser
 */

// constants won't change. They're used here to 
// set pin numbers:
const int laserbuttonPin = 17;     // the number of the pushbutton pin
const int laserPin =  18;      // the number of the LED pin

// variables will change:
int laserbuttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(laserPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(laserbuttonPin, INPUT);     
}

void loop(){
  // read the state of the pushbutton value:
  int laserbuttonState1 = digitalRead(laserbuttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (laserbuttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(laserPin, LOW);  
  } 
  else {
    // turn LED off:
    digitalWrite(laserPin, HIGH); 
  }
 
}