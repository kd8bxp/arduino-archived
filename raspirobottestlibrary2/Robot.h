/*
RaspPiRobot V3f Library for Arduino and Raspberry PI Board Core
by LeRoy Miller (C)2018
*/

#ifndef Robot_h
#define Robot_h

#include "Arduino.h"

//Arduino uses GPIO Pin number system
#define LEFT_PWM_PIN 24
#define LEFT_1_PIN 17
#define LEFT_2_PIN 4
#define RIGHT_PWM_PIN 14
#define RIGHT_1_PIN 10
#define RIGHT_2_PIN 25
#define TRIGGER_PIN 18
#define ECHO_PIN 23

class Robot {
private:
  unsigned long _previousMicros = 0;
  unsigned long _intervalMicros = 5000; //update PWM
  unsigned long _previousMillis = 0;
  long _onTime;
  int _speedL;
  int _speedR;
  int _isrunning;
  long _duration;
  int _distance;
  void updatePWM();
  void pwm(int pin, int del);
public:
Robot();
void begin();
void speed(int speedL, int speedR);
void stop();
int ping(); //returns in inches
void forward(int onTime);
void backward(int onTime);
void turnLeft(int onTime);
void turnRight(int onTime);
void update();
int isRunning();

};

#endif
