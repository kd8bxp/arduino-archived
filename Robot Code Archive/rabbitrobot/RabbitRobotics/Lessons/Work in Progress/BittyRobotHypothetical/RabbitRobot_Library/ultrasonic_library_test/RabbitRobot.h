/*
	RabbitRobot.com Library v1.0 Copyright (C) LeRoy Miller Aug 9, 2017

Next Step: Move functions from robot sketches, and create a library.

*/

#ifndef RabbitRobot_h
#define RabbitRobot_h

#include "Arduino.h"

//Name of the robot object.
class RabbitRobot {
//This setups the variables that are used by the library, private is only for the library and normally starts
//with a underscore to make it easy to see which variables are private and which are not.
private:
	//Motor Drive variables
	int _L_PIN; //Non-PWM Left wheel
	int _L_PWM; //PWM Left wheel (speed control)
	int _R_PIN; //Non-PWM Right wheel
	int _R_PWM; //PWM Right wheel (speed control)
	//Generally, you should use "unsigned long" for variables that hold time
	//The value will quickly become too large for an int to store
	unsigned long _previousMillis; //store the last time the motor were updated.
	unsigned long _currentMillis; //used to store a snap shot of the current millisec at the time the update function is called.
	int _isRunning; //set flag if motors are running.
	int _onTime; //this is used as a check against time
	
//you will notice in the above we copied our variables from the previous sketch and added a underscore to the
//beginning.
//In this case public is used to invoke the library, and takes the place of functions (subroutines) in the 
//previous sketch.  It will become clear what these are and how to use them a little later.
//You will notice, we have added a couple more functions as well. 
public:
RabbitRobot (int L_PIN, int L_PWM, int R_PIN, int R_PWM);
void begin();
void stop();
void forward(int speed, int duration);
void backwards(int speed, int duration);
void right(int speed, int duration);
void left(int speed, int duration);
void update();
int IsRunning();
};

//We will make a second class for using the Ultrasonic with the library
class UltraSonic {
private:
	//Ultrasonic Variables
	int _triggerPin;
	int _echoPin;	
	float _pingTime;  //time for ping to travel from sensor to target and return
	float _targetDistance; //Distance to Target in inches
	float _speedOfSound; //Speed of sound in miles per hour when temp is 77 degrees.
	unsigned long _echoDuration; //Used to store the duration of the echo in microseconds 
	int _DURATION_PER_INCH; //The approximate duration in microseconds of the echo for an object one inch away.
	int _pingavg; //avgerage ping distances
	int _pingcount; //ping counter
  int _avgCount;
public:
UltraSonic (int triggerPin, int echoPin);
void begin();
void ping();
void avgClear(); //resets the average count and average distance to zero
int avgCount(); //returns avgerage count
int avgDistance(); //returns the avgerage distance in inches
int targetDistance(); //returns the non-avgeraged distance in inches
};

#endif
