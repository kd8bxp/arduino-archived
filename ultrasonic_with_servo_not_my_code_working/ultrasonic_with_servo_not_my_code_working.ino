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

#define TRIG 12
#define ECHO 11

#define HEAD_SERVO 8

#define OBSTRACLE_DISTANCE 30.0
#define TURN_DELAY 200


#define LOG false

NewPing ultrasonic(TRIG, ECHO, 200);
Servo headServo;

int servoAngle = 90;
int angleStep = 30;

float distance = 0;

void setup()
{
  pinMode(MOTOR_1_FORWARD, OUTPUT);
  pinMode(MOTOR_1_BACK,    OUTPUT);
  pinMode(MOTOR_2_FORWARD, OUTPUT);
  pinMode(MOTOR_2_BACK,    OUTPUT);
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
  digitalWrite(MOTOR_1_FORWARD, HIGH);
  digitalWrite(MOTOR_1_BACK,    LOW);
  digitalWrite(MOTOR_2_FORWARD, HIGH);
  digitalWrite(MOTOR_2_BACK,    LOW);
}

void goBack()
{
  digitalWrite(MOTOR_1_FORWARD, LOW);
  digitalWrite(MOTOR_1_BACK,    HIGH);
  digitalWrite(MOTOR_2_FORWARD, LOW);
  digitalWrite(MOTOR_2_BACK,    HIGH);
}

void goLeft()
{
  digitalWrite(MOTOR_1_FORWARD, LOW);
  digitalWrite(MOTOR_1_BACK,    HIGH);
  digitalWrite(MOTOR_2_FORWARD, HIGH);
  digitalWrite(MOTOR_2_BACK,    LOW);
}

void goRight()
{
  digitalWrite(MOTOR_1_FORWARD, HIGH);
  digitalWrite(MOTOR_1_BACK,    LOW);
  digitalWrite(MOTOR_2_FORWARD, LOW);
  digitalWrite(MOTOR_2_BACK,    HIGH);
}

void stopMove()
{
  digitalWrite(MOTOR_1_FORWARD, LOW);
  digitalWrite(MOTOR_1_BACK,    LOW);
  digitalWrite(MOTOR_2_FORWARD, LOW);
  digitalWrite(MOTOR_2_BACK,    LOW);
}
