#include <Robonii_Motor.h>


void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  pinMode(14, OUTPUT);    
  digitalWrite(14, HIGH);    // set the LED off
  Robonii_Motors.InitMotor();
}

void loop() {
  digitalWrite(13, HIGH);   // set the LED on
  delay(2000);              // wait for a second
  //BothForward(3000);
 //Robonii_Motors.MotorPower(Motor_Left,POWER_FREE);
 Robonii_Motor_Left.Free();
 
  digitalWrite(13, LOW);    // set the LED off
  delay(2000);              // wait for a second
  //Robonii_Motors.MotorPower(Motor_Left,POWER_BREAK);
  Robonii_Motor_Left.Brake();
  
   delay(2000);              // wait for a second
  //Robonii_Motors.MotorPower(Motor_Left,90);
  //Robonii_Motors.MotorPower(Motor_Right,90);
  Robonii_Motor_Left.SetSpeed(90);
  Robonii_Motor_Right.SetSpeed(90);
  
   delay(2000);              // wait for a second
  //Robonii_Motors.MotorPower(Motor_Left,-90);
  //Robonii_Motors.MotorPower(Motor_Right,-90);
  Robonii_Motors.SetSpeed(-90);
  
 // BothReverse(2000);
}

