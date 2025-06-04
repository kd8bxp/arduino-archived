
/*
by Ray Scott for testing needs
for testing the Compact Rover and Bitty Rover
...
Connected to the L298 motor driver
 */

// include the library code:
#include <Servo.h>

Servo servo9G;

const int RightWheelA = 4; //digital pin
const int RightWheelB = 5; //PWM pin
const int LeftWheelA = 6; //PWM pin
const int LeftWheelB = 7; //digital pin
// this is the 4-wire mode without using actual PWM 


void setup() {

  servo9G.attach(3); //analog pin 0
   
   // Motor Driver Setup
   pinMode(RightWheelA, OUTPUT);
   pinMode(RightWheelB, OUTPUT);
   pinMode(LeftWheelA, OUTPUT);
   pinMode(LeftWheelB, OUTPUT);
      
// end of "void setup" 
}

void loop() {
   GoForward();
 //  servo9G.write(1500);
   delay(250); // turning time
   GoStop();
   delay(100);
   
   GoRight();
 //  servo9G.write(1000);
   delay(250); // turning time
   GoStop();
   delay(100);

   GoLeft();
 //  servo9G.write(2000);
   delay(250); // turning time
   GoStop();
   delay(100);
   
   GoRight();
 //  servo9G.write(1000);
   delay(250); // turning time
   GoStop();
   delay(100);

   GoLeft();
//   servo9G.write(2000);
   delay(250); // turning time
   GoStop();
   delay(100);
   
   GoRight();
//   servo9G.write(1000);
   delay(250); // turning time
   GoStop();
   delay(100);

   GoLeft();
//   servo9G.write(2000);
   delay(150); // turning time
   GoStop();
   delay(100);
   
   
   SpinRight();
   delay(700); // turning time
   
   
// end of main void loop
}


// now have special functions

void SpinLeft(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,HIGH);
  digitalWrite(RightWheelB,HIGH);
  digitalWrite(RightWheelA,LOW);
}

void GoLeft(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelB,HIGH);
  digitalWrite(RightWheelA,LOW);
}

void SpinRight(){
  digitalWrite(LeftWheelA,HIGH);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelA,HIGH);
  digitalWrite(RightWheelB,LOW);
}

void GoRight(){
  digitalWrite(LeftWheelA,HIGH);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelA,LOW);
  digitalWrite(RightWheelB,LOW);
}


void GoReverse(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,HIGH);
  digitalWrite(RightWheelB,LOW);
  digitalWrite(RightWheelA,HIGH);


}
void GoForward(){
  digitalWrite(LeftWheelA,HIGH);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelB,HIGH);
  digitalWrite(RightWheelA,LOW);
}

void GoStop(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelB,LOW);
  digitalWrite(RightWheelA,LOW);
}

