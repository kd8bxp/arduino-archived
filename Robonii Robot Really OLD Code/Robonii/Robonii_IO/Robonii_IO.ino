/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

/*
Find the Robonii pre-DEFINE's in :C:\...\arduino-1.0.1-rc2-xmegaduino-beta4\hardware\xmegaduino\variants\Robonii\Robonii.h"
*/
void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);  
pinMode(D5_Green, OUTPUT);  
  
}

void loop() {
  digitalWrite(13, HIGH);   // set the LED on
  digitalWrite(D5_Green, HIGH);   // set the LED on
  
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  digitalWrite(D5_Green, LOW);    // set the LED off
  
  delay(1000);              // wait for a second
}
