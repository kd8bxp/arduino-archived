// Motor Controls Link to Original Article
// Automation Technology Club
// Motor Controls
// Free to use as you see if - Pin Numbering by Grand Poobah
// Subroutines Aug 18 - LeRoy Miller
// additions will be added for speed correction of wheels

//Aug 22 2015 - added ultrasonic sensor using NewPing Library https://code.google.com/p/arduino-new-ping/

#include <NewPing.h>
#include <Servo.h>

const int PWMLPin = 44; //	Enable PWM Left Motor
const int PWMRPin = 46; //	Enable PWM Right Motor
const int L1Pin = 36; // 	L1 = Left Motor Lead 1
const int L2Pin = 38; // 	L2 = Left Motor Lead 2
const int L3Pin = 40; // 	L3 = Right Motor Lead 1
const int L4Pin = 42; // 	L4 = Right Motor Lead 2
const int LEDPin = 13; // LED
const int MaxSpeed = 255; //not yet used, this will be the max speed your motors can drive at (max PWM) and will be used to help correct for differences in motors
const int CorrectLeft = 0; //to be added later, this will attempt to correct for any motor differances in going forward/backward
const int CorrectRight = 0; //to be added later, this will attempt to correct for any motor differances in going forward/backward
int speed; 

#define ServoPin 13 //attach the servo to this pin

int servoAngle = 90;
int angleStep = 30;
float distance = 0;
#define OBSTRACLE_DISTANCE 30

/* FUNCTION CALL USES
lefttight();  //turn in place to the left
righttight(); //turn in place to the right
left();   //turn left going forward
leftb();  //turn left going backward
allstop(); //stop all motors and pwm, small delay for everything to catch up
right(); //turn right going forward
rightb(); //turn right going backward
forward(); //move the robot forward
backward(); // move the robot backward
circleleft(100); //make a left hand circle X number of times
circleright(100); //make a right hand circle  X number of times
*/

#define TRIGGER_PIN 12 //Arduino Pin Tied to Trigger Pin on the ultrasonic sensor
#define ECHO_PIN 11 //Arduino Pin Tied to echo pin on the ultrasonic sensor
#define MAX_DISTANCE 200 //Maximum distance we want to ping for (in centimeters).  Maximum sensor distance is rated at 400 - 500cm

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

//Bitty Bot will drive forward until something gets in it way - slowing down as it gets close to an object at aproxmately 2 inches it will then backup, turn to the left, and try to move forward again.

void setup()
{

pinMode (L1Pin, OUTPUT);
pinMode (L2Pin, OUTPUT);
pinMode (L3Pin, OUTPUT);
pinMode (L4Pin, OUTPUT);
pinMode (PWMLPin, OUTPUT);
pinMode (PWMRPin, OUTPUT);

pinMode (ServoPin, OUTPUT);
myservo.attach(ServoPin); 

}

void loop()
{

myservo.write(90);
speed = 150; // fastest I want Bitty Bot to go
  delay(500);
  unsigned int uS = sonar.ping();
  unsigned int sI = (uS / US_ROUNDTRIP_IN);
  
  if (sI >= 30) {
    forward();
    delay(500);
  }
  
  if (sI > 20 && sI <= 29) {
    speed = 100;
    forward();
    delay(500);
  }
  
  if (sI > 6 && sI <= 19) {
    speed = 65;
    forward();
    delay(500);
  }
  
  if (sI <=5) {
    allstop();
    delay(500);
    speed = 150;
    backward();
    delay(700);
    allstop();
    updateHeadAngle();
   checkObstracle();
   // lefttight();
    //delay(700);
    allstop();
  }
}

void lefttight() {
	digitalWrite (L1Pin, 1);
	digitalWrite (L2Pin, 0);
	analogWrite (PWMLPin, speed);
	digitalWrite (L3Pin, 0);
	digitalWrite (L4Pin, 1);
	analogWrite (PWMRPin, speed);
}

void left() {
	digitalWrite (L1Pin, 0);
	digitalWrite (L2Pin, 0);
	digitalWrite (L3Pin, 0);
	digitalWrite (L4Pin, 1);
	analogWrite(PWMLPin, 0);
	analogWrite(PWMRPin, speed);
	
}

void right() {
	digitalWrite (L1Pin, 0);
	digitalWrite (L2Pin, 1);
	digitalWrite (L3Pin, 0);
	digitalWrite (L4Pin, 0);
	analogWrite(PWMLPin, speed);
	analogWrite(PWMRPin, 0);
}

void leftb() {
	digitalWrite (L1Pin, 0);
	digitalWrite (L2Pin, 0);
	digitalWrite (L3Pin, 1);
	digitalWrite (L4Pin, 0);
	analogWrite(PWMLPin, 0);
	analogWrite(PWMRPin, speed);
}

void rightb() {
	digitalWrite (L1Pin, 1);
	digitalWrite (L2Pin, 0);
	digitalWrite (L3Pin, 0);
	digitalWrite (L4Pin, 0);
	analogWrite(PWMLPin, speed);
	analogWrite(PWMRPin, 0);
}
int righttight() {
	digitalWrite(L1Pin, 0);
	digitalWrite(L2Pin, 1);
	analogWrite (PWMLPin, speed);
	digitalWrite(L3Pin, 1);
	digitalWrite(L4Pin, 0);
	analogWrite(PWMRPin, speed);
}

int backward() {
	digitalWrite(L1Pin, 1);
	digitalWrite(L2Pin, 0);
	digitalWrite(L3Pin, 1);
	digitalWrite(L4Pin, 0);
	analogWrite(PWMLPin, speed);
	analogWrite(PWMRPin, speed);
}

int forward() {
	digitalWrite(L1Pin, 0);
	digitalWrite(L2Pin, 1);
	digitalWrite(L3Pin, 0);
	digitalWrite(L4Pin, 1);
	analogWrite(PWMLPin, speed);
	analogWrite(PWMRPin, speed);
	
}

int allstop() {
	digitalWrite(L1Pin, 0);
	digitalWrite(L2Pin, 0);
	digitalWrite(L3Pin, 0);
	digitalWrite(L4Pin, 0);
	analogWrite(PWMLPin, 0);
	analogWrite(PWMRPin, 0);
delay(500);

}

int circleleft(int count) {
	for (int i=0; i<=count; i++) {
		left();
	delay(250);	
	}
	allstop();
}

int circleright(int count) {
	for (int i=0; i<=count; i++) {
		right();
		delay(250);
	}
	allstop();
}

void checkObstracle()
{
  int obsLeft  = 0;
  int obsRight = 0;
  
  // Count the obstacles from left and right side
  for(servoAngle = 0; servoAngle <= 180; servoAngle += 30)
  {
    myservo.write(servoAngle);
    delay(400);
    
    int uS = sonar.ping();
  distance = (uS/ US_ROUNDTRIP_CM);
  
    if(distance < OBSTRACLE_DISTANCE && servoAngle < 90) obsLeft++;
    else if(distance < OBSTRACLE_DISTANCE) obsRight++;
  }
  
  if(obsLeft && obsRight)
  {
    backward();
    delay(700);
    
    if(obsLeft < obsRight) lefttight(); 
    else righttight();
    delay(700);
  }
  else if(obsRight)
  {
    lefttight();
    delay(700);
  }
  else if(obsLeft)
  {
    righttight();
    delay(700);
  }
  else
  {
    forward();
    delay(500);
  }
}

void updateHeadAngle()
{
  myservo.write(servoAngle);
  servoAngle += angleStep;
  
  if(servoAngle >= 150)
  {
    servoAngle = 150;
    angleStep *= -1;
  }
  
  if(servoAngle <= 30)
  {
    servoAngle = 30;
    angleStep *= -1;
  }


  
}