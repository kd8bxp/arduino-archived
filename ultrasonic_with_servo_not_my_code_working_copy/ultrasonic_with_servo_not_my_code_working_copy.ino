//code from this youtube video
//https://www.youtube.com/watch?v=gfEOJ4F91E8
//changed code for my mega, changed code to use NewPing
//seems to work, but too fast need to add speed control to it
//Original code believed to be by Santosh Kulkami on Mar 10, 2013 
//my modifications done by LeRoy Miller - Aug 29, 2015

#include <NewPing.h>
#include <Servo.h>

#define MOTOR_1_FORWARD 36
#define MOTOR_1_BACK 38
#define MOTOR_2_FORWARD 40
#define MOTOR_2_BACK 42

#define PWMLPin 46
#define PWMRPin 44

#define TRIG 12
#define ECHO 11

#define HEAD_SERVO 13

#define OBSTRACLE_DISTANCE 30.0
#define TURN_DELAY 200


#define LOG false

NewPing ultrasonic(TRIG, ECHO, 200);
Servo headServo;

int servoAngle = 90;
int angleStep = 30;

float distance = 0;
int speed = 155;

void setup()
{
  pinMode(MOTOR_1_FORWARD, OUTPUT);
  pinMode(MOTOR_1_BACK,    OUTPUT);
  pinMode(MOTOR_2_FORWARD, OUTPUT);
  pinMode(MOTOR_2_BACK,    OUTPUT);
  pinMode(PWMLPin, OUTPUT);
  pinMode(PWMRPin, OUTPUT);
digitalWrite(44, HIGH);
digitalWrite(46, HIGH);  
  stopMove();
  
  headServo.attach(HEAD_SERVO); 
  
  if(LOG) Serial.begin( 9600 );
}

void loop()
{
  updateHeadAngle();
  
  checkDistance();
  moove();
  
  delay(10);
}

void checkDistance()
{
 int uS = ultrasonic.ping();
  distance = (uS/ US_ROUNDTRIP_CM);
  if(LOG) Serial.println(distance); 

if (distance >= 30) {speed = 155;}
if (distance >= 15 && distance <= 29) {speed = 100;}
if (distance >=6 && distance <=14) {speed = 65;}
}

void moove()
{
  if( distance > OBSTRACLE_DISTANCE )
  {
    if(LOG) Serial.println("FORWARD"); 
    
    goForward();
    delay(TURN_DELAY);
  }
  else
  {
    stopMove();
    
    checkObstracle();
  }
}

void checkObstracle()
{
  int obsLeft  = 0;
  int obsRight = 0;
  
  // Count the obstacles from left and right side
  for(servoAngle = 0; servoAngle <= 180; servoAngle += 30)
  {
    headServo.write(servoAngle);
    delay(TURN_DELAY);
    
    checkDistance();
    if(distance < OBSTRACLE_DISTANCE && servoAngle < 90) obsLeft++;
    else if(distance < OBSTRACLE_DISTANCE) obsRight++;
  }
  
  if(LOG) Serial.print("TURN"); 
    
  if(obsLeft && obsRight)
  {
    goBack();
    
    delay(TURN_DELAY * 2);
    
    if(obsLeft < obsRight) goLeft();
    else goRight();
    
    delay(TURN_DELAY);
  }
  else if(obsRight)
  {
    goLeft();
    
    delay(TURN_DELAY);
  }
  else if(obsLeft)
  {
    goRight();
    
    delay(TURN_DELAY);
  }
  else
  {
    goForward();
    
    delay(TURN_DELAY);
  }
}

void updateHeadAngle()
{
  headServo.write(servoAngle);
  
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

void goForward()
{
	delay(500);
  digitalWrite(MOTOR_1_FORWARD, 0);
  analogWrite(MOTOR_1_BACK, speed);
  //digitalWrite(MOTOR_1_BACK,    1);
  digitalWrite(MOTOR_2_FORWARD, 0);
  //digitalWrite(MOTOR_2_BACK,    1);
  analogWrite(MOTOR_2_BACK, speed);
  //analogWrite(46, speed);
  //analogWrite(44, speed);
}

void goBack()
{
	delay(500);
 // digitalWrite(MOTOR_1_FORWARD, 1);
  analogWrite(MOTOR_1_FORWARD, speed); 
  digitalWrite(MOTOR_1_BACK,    0);
  analogWrite(MOTOR_2_FORWARD, speed);
  //digitalWrite(MOTOR_2_FORWARD, 1);
  digitalWrite(MOTOR_2_BACK,    0);
  //analogWrite(46, speed);
  //analogWrite(44, speed);
}

void goLeft()
{
	delay(500);
  //digitalWrite(MOTOR_1_FORWARD, HIGH);
  analogWrite(MOTOR_1_FORWARD, speed);
  digitalWrite(MOTOR_1_BACK,    LOW);
  digitalWrite(MOTOR_2_FORWARD, LOW);
  analogWrite(MOTOR_2_BACK, speed);
  //digitalWrite(MOTOR_2_BACK,    HIGH);
  //analogWrite(PWMLPin, speed);
  //analogWrite(PWMRPin, speed);
}

void goRight()
{
	delay(500);
  digitalWrite(MOTOR_1_FORWARD, 0);
  analogWrite(MOTOR_1_BACK, speed);
  //digitalWrite(MOTOR_1_BACK,    1);
  analogWrite(MOTOR_2_FORWARD, speed);
 // digitalWrite(MOTOR_2_FORWARD, 1);
  digitalWrite(MOTOR_2_BACK,    0);
 // analogWrite(PWMLPin, speed);
 // analogWrite(PWMRPin, speed);
}

void stopMove()
{
	delay(500);
  digitalWrite(MOTOR_1_FORWARD, LOW);
  digitalWrite(MOTOR_1_BACK,    LOW);
  digitalWrite(MOTOR_2_FORWARD, LOW);
  digitalWrite(MOTOR_2_BACK,    LOW);
  //analogWrite(PWMLPin, 0);
  //analogWrite(PWMRPin, 0);
}
