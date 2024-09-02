/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <ServoTimer2.h>  // the servo library
#define left1 12
#define left2 11 //pwm
#define right1 10
#define right2 9 //pwm
ServoTimer2 myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

void setup() {
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
 pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  stop(); //make sure motors are stopped
  myservo.write(1500);              // tell servo to go to position in variable 'pos'
cw (255,255);
delay(5000);
myservo.write(2000);
stop();
ccw(60,100);
delay(5000);
stop();
myservo.write(850);
cw(60,60);
delay(5000);
stop();
}

void loop() {
  
   
}

// this function just increments a value until it reaches a maximum 
int incPulse(int val, int inc){
   if( val + inc  > 2000 )
      return 1000 ;
   else
       return val + inc;  
}
void cw(int lft, int rgt) { 
  //This causes the car to move backward
  digitalWrite(left1, 1);
  digitalWrite(right1, 1);
  analogWrite(left2, map(lft,0,255,255,0));
  analogWrite(right2,map(rgt,0,255,255,0));  
}

void ccw(int lft, int rgt) {
  //This causes the car to move forward
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  analogWrite(left2, lft);
  analogWrite(right2, rgt);
}

void stop() {
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  analogWrite(left2, 0);
  analogWrite(right2, 0);
}


