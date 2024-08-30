#include <xTimer.h>

// xTimer makes the variable TimerD1A available;
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly. 
  This example code is in the public domain.
 */

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(21, OUTPUT);  
  
  pinMode(13, OUTPUT);      
  pinMode(15, OUTPUT);
  pinMode(17, OUTPUT);
  
  //Inititialize the Timer to excecute every 1ms (1000 * 1us)
  TimerD1A.initialize(1000);
  
  // Initialize the 3x callbacks to blink at diffrent intervals...
  TimerD1A.attachInterrupt1(Blink,6000);
  TimerD1A.attachInterrupt2(Blink2,3000);
  TimerD1A.attachInterrupt3(Blink3,1000);
}

void loop() {
  digitalWrite(21, LOW);   // set the LED on
  delay(500);              // wait for a second
  digitalWrite(21,HIGH );    // set the LED off
  delay(500);              // wait for a second
}

int TXdata = 0;
void Blink() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
 //
TXdata = TXdata + 1;
 if (TXdata > 1)
 {
 digitalWrite(13, LOW);   // set the LED OFF
  TXdata = 0;

 }
 if (TXdata == 1)
 {   
   digitalWrite(13, HIGH);   // set the LED on
 } 
}

int TXdata2 = 0;
int DetachAfterBlinks = 10;
void Blink2() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
 //
TXdata2 = TXdata2 + 1;
 if (TXdata2 > 1)
 {
 digitalWrite(15, LOW);   // set the LED OFF
  TXdata2 = 0;

 }
 if (TXdata2 == 1)
 {   
   digitalWrite(15, HIGH);   // set the LED on
 } 
 DetachAfterBlinks--;
   if (DetachAfterBlinks==0)
   {
     TimerD1A.detachInterrupt(2);
   }
 
}

int TXdata3 = 0;
void Blink3() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
 //
TXdata3 = TXdata3 + 1;
 if (TXdata3 > 1)
 {
 digitalWrite(17, LOW);   // set the LED OFF
  TXdata3 = 0;

 }
 if (TXdata3 == 1)
 {   
   digitalWrite(17, HIGH);   // set the LED on
 } 
}
