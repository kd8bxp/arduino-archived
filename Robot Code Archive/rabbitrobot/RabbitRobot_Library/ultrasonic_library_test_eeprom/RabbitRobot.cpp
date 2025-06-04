/*
	RabbitRobot.com Library v1.0 Copyright (C) LeRoy Miller Aug 9, 2017
Next Step: Move functions from robot sketches, and create a library.
Should show how to create a library from exsiting functions and subroutines
*/

#include "Arduino.h" //this is so we can use the Arduino PINs
#include "RabbitRobot.h"
#include <EEPROM.h>

RabbitRobot::RabbitRobot(String prom) {
  //Address 172 is the check byte - if set we have eeprom information to load.
  //Address 173 is the number of wires used (4 or 6)
  //Address 174 is the Left Motor (NonPWM) Pin
  //Address 175 is the Left Motor (PWM) Pin
  //Address 176 is the Right Motor (NonPWM) Pin
  //Address 177 is the Right Motor (PWM) Pin
  //Address 178 is the Left Enable Pin (IF 6 wire is used)
  //Address 179 is the Right Enable Pin (IF 6 wire is used)
  
  if (prom == "READ") {
    int _check = EEPROM.read(172);
    if (_check == 1) {
      int WIRE = EEPROM.read(173);
      _L_PIN = EEPROM.read(174);
      _L_PWM = EEPROM.read(175);
      _R_PIN = EEPROM.read(176);
      _R_PWM = EEPROM.read(177);
      if (WIRE == 6) {
        _L_EN = EEPROM.read(178);
        _R_EN = EEPROM.read(179);
        _WIRE6 = 1;
      } 
    } else {
        Serial.println("EEPROM not set!");
        while(1) {}
      }
    _previousMillis = 0;
    _isRunning = 0;
  }
  if (prom == "WRITE") {
   
  }
  if (prom == "UPDATE") {
    
  }
  if (prom == "CLEAR") {
    for (int _i=0; _i< EEPROM.length(); _i++) {
      EEPROM.write(_i,0);
      Serial.println("Memory Cleared!");
      while(1) {}
    }
  }
}

RabbitRobot::RabbitRobot(String prom, int L_PIN, int L_PWM, int R_PIN, int R_PWM) {
   //Address 172 is the check byte - if set we have eeprom information to load.
  //Address 173 is the number of wires used (4 or 6)
  //Address 174 is the Left Motor (NonPWM) Pin
  //Address 175 is the Left Motor (PWM) Pin
  //Address 176 is the Right Motor (NonPWM) Pin
  //Address 177 is the Right Motor (PWM) Pin
  //Address 178 is the Left Enable Pin (IF 6 wire is used)
  //Address 179 is the Right Enable Pin (IF 6 wire is used)
  
  if (prom == "WRITE" || prom == "UPDATE") {
    EEPROM.update(172, 1);
    EEPROM.update(173, 4);
    EEPROM.update(174, L_PIN);
    EEPROM.update(175, L_PWM);
    EEPROM.update(176, R_PIN);
    EEPROM.update(177, R_PWM);
  
//This is used to construct a Robot object, variables such as pin numbers for the motors
//are passed and stored for the library here. Other conditions are also set to the default/start values
_L_PIN = L_PIN; //Non-PWM Left Wheel
_L_PWM = L_PWM; //PWM Left Wheel (speed control)
_R_PIN = R_PIN; //Non-PWM Right Wheel
_R_PWM = R_PWM; //PWM Right Wheel (speed control)
_previousMillis = 0; //previousMillis has not been used yet, it's start should be zero
_isRunning = 0; //the Robot is not running.
  }
  Serial.println("EEPROM UPDATED.");
  while(1) {}
}

RabbitRobot::RabbitRobot(String prom, int L_PIN, int L_PWM, int R_PIN, int R_PWM, int L_EN, int R_EN) {
   //Address 172 is the check byte - if set we have eeprom information to load.
  //Address 173 is the number of wires used (4 or 6)
  //Address 174 is the Left Motor (NonPWM) Pin
  //Address 175 is the Left Motor (PWM) Pin
  //Address 176 is the Right Motor (NonPWM) Pin
  //Address 177 is the Right Motor (PWM) Pin
  //Address 178 is the Left Enable Pin (IF 6 wire is used)
  //Address 179 is the Right Enable Pin (IF 6 wire is used)
  
  if (prom == "WRITE" || prom == "UPDATE") {
    EEPROM.update(172, 1);
    EEPROM.update(173, 6);
    EEPROM.update(174, L_PIN);
    EEPROM.update(175, L_PWM);
    EEPROM.update(176, R_PIN);
    EEPROM.update(177, R_PWM);
    EEPROM.update(178, L_EN);
    EEPROM.update(179, R_EN);
//This is used to construct a Robot object, variables such as pin numbers for the motors
//are passed and stored for the library here. Other conditions are also set to the default/start values
_L_PIN = L_PIN; //Non-PWM Left Wheel
_L_PWM = L_PWM; //PWM Left Wheel (speed control)
_R_PIN = R_PIN; //Non-PWM Right Wheel
_R_PWM = R_PWM; //PWM Right Wheel (speed control)
_L_EN = L_EN;
_R_EN = R_EN;
_WIRE6 = 1;
_previousMillis = 0; //previousMillis has not been used yet, it's start should be zero
_isRunning = 0; //the Robot is not running.
  }
  Serial.println("EEPROM UPDATED.");
  while(1) {}
}


RabbitRobot::RabbitRobot(int L_PIN, int L_PWM, int R_PIN, int R_PWM) {
//This is used to construct a Robot object, variables such as pin numbers for the motors
//are passed and stored for the library here. Other conditions are also set to the default/start values
_L_PIN = L_PIN; //Non-PWM Left Wheel
_L_PWM = L_PWM; //PWM Left Wheel (speed control)
_R_PIN = R_PIN; //Non-PWM Right Wheel
_R_PWM = R_PWM; //PWM Right Wheel (speed control)
_previousMillis = 0; //previousMillis has not been used yet, it's start should be zero
_isRunning = 0; //the Robot is not running.
}

RabbitRobot::RabbitRobot(int L_PIN, int L_PWM, int R_PIN, int R_PWM, int L_EN, int R_EN) {
//This is used to construct a Robot object, variables such as pin numbers for the motors
//are passed and stored for the library here. Other conditions are also set to the default/start values
_L_PIN = L_PIN; //Non-PWM Left Wheel
_L_PWM = L_PWM; //PWM Left Wheel (speed control)
_R_PIN = R_PIN; //Non-PWM Right Wheel
_R_PWM = R_PWM; //PWM Right Wheel (speed control)
_L_EN = L_EN;
_R_EN = R_EN;
_WIRE6 = 1;
_previousMillis = 0; //previousMillis has not been used yet, it's start should be zero
_isRunning = 0; //the Robot is not running.
}

void RabbitRobot::begin() {
//Sets all the PINs to OUTPUT
	pinMode(_L_PIN, OUTPUT);
	pinMode(_L_PWM, OUTPUT);
	pinMode(_R_PIN, OUTPUT);
	pinMode(_R_PWM, OUTPUT);
  if (_WIRE6) {
    pinMode(_L_EN, OUTPUT);
    pinMode(_R_EN, OUTPUT);
  }
//makes sure the motors really are stopped
	digitalWrite(_L_PIN, LOW);
	analogWrite(_L_PWM, 0);
	digitalWrite(_R_PIN, LOW);
	analogWrite(_R_PWM, 0);
 if (_WIRE6) {
  digitalWrite(_L_EN, LOW);
  digitalWrite(_R_EN, LOW);
 }
}

//The rest of the motor control is a copy of the functions from our previous sketch, with the variables
//changed to have a underscore in the names

void RabbitRobot::left(int speed, int duration) {
    _previousMillis = millis();
    digitalWrite(_R_PIN, LOW); 
    analogWrite(_R_PWM, speed);
    if (_WIRE6) {digitalWrite(_R_EN, HIGH);}
    _onTime = duration; //notice the replacement of delay with the variable onTime.
    _isRunning = 1; //motors should be running
}

void RabbitRobot::right(int speed, int duration) {
    _previousMillis = millis();
    digitalWrite(_L_PIN, LOW); 
    analogWrite(_L_PWM, speed);
    if (_WIRE6) {digitalWrite(_L_EN, HIGH); }
    _onTime = duration; //notice the replacement of delay with the variable onTime.
    _isRunning = 1; //motors should be running
}

void RabbitRobot::forward(int speed, int duration) {
    _previousMillis = millis();
    digitalWrite(_L_PIN, LOW); //Set both the left and right motors to go forwards
    analogWrite(_L_PWM, speed);
    digitalWrite(_R_PIN, LOW);
    analogWrite(_R_PWM, speed);
    if (_WIRE6) {
      digitalWrite(_L_EN, HIGH);
      digitalWrite(_R_EN, HIGH);
    }
    _onTime = duration; //notice the replacement of delay with the variable onTime.
    _isRunning = 1; //motors should be running
}

void RabbitRobot::backwards(int speed, int duration) {
    _previousMillis = millis();
    digitalWrite(_L_PIN, HIGH); //Set both the left and right motors to go in reverse
    analogWrite(_L_PWM, 255 - speed);
    digitalWrite(_R_PIN, HIGH);
    analogWrite(_R_PWM, 255 - speed);   
    if (_WIRE6) {
      digitalWrite(_L_EN, HIGH);
      digitalWrite(_R_EN, HIGH);
    }    
    _onTime = duration; //notice the replacement of delay with the variable onTime.
    _isRunning = 1; //motors should be running
}

void RabbitRobot::stop() {
    _previousMillis = millis(); //set timings to millisecs the function was called
    digitalWrite(_L_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(_L_PWM, LOW);
    digitalWrite(_R_PIN, LOW);
    digitalWrite(_R_PWM, LOW);
    if (_WIRE6) {
      digitalWrite(_L_EN, LOW);
      digitalWrite(_R_EN, LOW);
    }
    _isRunning = 0; //motors should not be running flag.
        }

void RabbitRobot::update() {
  _currentMillis = millis();
  if (_currentMillis - _previousMillis >= _onTime) {
    _previousMillis = _currentMillis;
    RabbitRobot::stop();
  }
}

int RabbitRobot::IsRunning() {
	return(_isRunning);
};

//We are going to do the same thing for the Ultrasonic class we created in the RabbitRobots.h File

UltraSonic::UltraSonic (int triggerPin, int echoPin) {
_triggerPin = triggerPin;
_echoPin = echoPin;
}

void UltraSonic::begin() {
//set the pins and initilize the variables
_speedOfSound=776.5; //Speed of sound in miles per hour when temp is 77 degrees.
_echoDuration = 0; //Used to store the duration of the echo in microseconds 
_DURATION_PER_INCH = 550; //The approximate duration in microseconds of the echo for an object one inch away.
_pingavg = 0; //avgerage ping distances
_pingcount = 0; //ping counter

pinMode(_triggerPin, OUTPUT);
pinMode(_echoPin, INPUT);

}

void UltraSonic::ping() {
  _pingcount ++; //increase counter by one
  digitalWrite(_triggerPin, LOW);
  delayMicroseconds(2000);
  digitalWrite(_triggerPin, HIGH); //Send voltage to the trigger pin to begin the ultrasound pusle
  delayMicroseconds(15); //Pause the program for 10 microseconds (to produce a 10 microsecond pulse)
  digitalWrite(_triggerPin, LOW); //Stop sendig voltage to the trigger pin, ending the pulse
  delayMicroseconds(10);
  _echoDuration = pulseIn(_echoPin, HIGH); //Get the duration of the echo coming back in microseconds 
  _pingTime = _echoDuration;
  _pingTime=_pingTime/1000000; //convert pingTime to seconds by dividing by 1000000 (microseconds in a second)
  _pingTime=_pingTime/3600; //convert pingtime to hourse by dividing by 3600 (seconds in an hour)
  _targetDistance= _speedOfSound * _pingTime;  //This will be in miles, since speed of sound was miles per hour
  _targetDistance=_targetDistance/2; //Remember ping travels to target and back from target, so you must divide by 2 for actual target distance.
  _targetDistance= _targetDistance*63360;    //Convert miles to inches by multipling by 63360 (inches per mile)
if (_targetDistance >= 100 || _targetDistance <= 0) {
  _pingavg = 0;
  _pingcount = 0;
} else {
  _pingavg = _pingavg + _targetDistance;
}
}

void UltraSonic::avgClear() {
_pingcount = 0;
_pingavg = 0;
}

int UltraSonic::avgCount() {
	return (_pingcount);
}

int UltraSonic::avgDistance() {
	return (_pingavg);
}

int UltraSonic::targetDistance() {
	return (_targetDistance);
};
