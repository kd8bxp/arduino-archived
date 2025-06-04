
/*
by Ray Scott for testing needs
for testing the Compact Rover and Bitty Rover

Uses the 4-wire mode for 2 DC motors of the L298 motor driver
 */

#include <Servo.h>

Servo servo9G;

const int RightWheelA = 4; //digital pin
const int RightWheelB = 5; //PWM pin
const int LeftWheelA = 6; //PWM pin
const int LeftWheelB = 7; //digital pin
// 4-wire mode will use PWM and /PWM

int PWMRight = 0;
int PWMLeft = 0;


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
//   GoForward();
 //  servo9G.write(1500); // ignore the servo during this test
//   delay(250); // turning time
//   GoStop();
//   delay(100);
   
   PWMRight = 225; // full speed
   PWMLeft  = 155; // half speed
   GoPWMForward(PWMRight, PWMLeft);
   delay(1000);
   
   GoStop();
   delay(100);

   PWMRight = 120; // ~half speed
   PWMLeft  = 255; // full speed
   GoPWMReverse(PWMRight, PWMLeft);
   delay(1000);

   GoStop();
   delay(100);

   PWMRight = 200; // 80% speed
   PWMLeft  = 200; // 80% speed
   PWMSpinRight(PWMRight, PWMLeft);
   delay(1000);


   GoStop();
   delay(100);

   PWMRight = 150; // half speed
   PWMLeft  = 150; // half speed
   PWMSpinLeft(PWMRight, PWMLeft); // 0 is slowest speed

   delay(1000);
   
   GoStop();
   delay(100);
   
}


// now have special functions


void GoPWMForward(int PWML, int PWMR) {
   digitalWrite(LeftWheelB,LOW);
   digitalWrite(RightWheelA,LOW);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

void GoPWMReverse(int PWML, int PWMR) {
  PWMR = 255-PWMR;
  PWML = 255-PWML;
  digitalWrite(LeftWheelB,HIGH);
   digitalWrite(RightWheelA,HIGH);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

void PWMSpinLeft(int PWML, int PWMR){
  PWML = 255-PWML;
   digitalWrite(LeftWheelB,HIGH);
   digitalWrite(RightWheelA,LOW);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

void PWMSpinRight(int PWML, int PWMR){
  PWMR = 255-PWMR;
   digitalWrite(LeftWheelB,LOW);
   digitalWrite(RightWheelA,HIGH);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

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

