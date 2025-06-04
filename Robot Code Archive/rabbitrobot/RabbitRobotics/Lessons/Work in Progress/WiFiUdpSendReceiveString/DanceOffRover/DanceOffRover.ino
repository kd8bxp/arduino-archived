
/*
by Ray Scott for testing needs
for testing the StingRay robot
 */


// include the library code:
// #include <LiquidCrystal.h>

#include <Servo.h>

Servo servo9G;



const int RightWheelA = 4; //need to change to PWM?
const int RightWheelB = 5; //PWM channel
const int LeftWheelA = 6; //need to change to PWM?
const int LeftWheelB = 7; //PWM channel
// only PWM with other channel LOW


int StepDelay = 255;
int StepDelayOrg = 255;
int RampRate = 5;
int RampDelay = 3;
int PWMRight = 0;
int PWMLeft = 0;
int MinSpeed = 1;
int MaxSpeed = 255;
int StepDirection = 1; // forward=1 .. reverse=0

void setup() {

 // pinMode(1,OUTPUT);
  servo9G.attach(3); //analog pin 0
  //servo9G.setMaximumPulse(2000);
  //servo9G.setMinimumPulse(700);
  

   
   // Motor Driver Setup
   pinMode(RightWheelA, OUTPUT);
   pinMode(RightWheelB, OUTPUT);
   pinMode(LeftWheelA, OUTPUT);
   pinMode(LeftWheelB, OUTPUT);
      
// end of "void setup" 
}

void loop() {
   GoForward();
   servo9G.write(1500);
   delay(50);
   GoStop();
   delay(300);
   
   GoRight();
   servo9G.write(1000);
   delay(50);
   GoStop();
   delay(300);

   GoLeft();
   servo9G.write(2000);
   delay(50);
   GoStop();
   delay(300);
   
   GoRight();
   servo9G.write(1000);
   delay(50);
   GoStop();
   delay(300);

   GoLeft();
   servo9G.write(2000);
   delay(50);
   GoStop();
   delay(300);
   
   GoRight();
   servo9G.write(1000);
   delay(50);
   GoStop();
   delay(300);

   GoLeft();
   servo9G.write(2000);
   delay(50);
   GoStop();
   delay(300);
   
   SpinRight();
   delay(600);
   
   

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

