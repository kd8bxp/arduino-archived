#include "Arduino.h"
#include "Robot.h"

Robot::Robot() {
}

void Robot::begin() {
  pinMode(LEFT_PWM_PIN, OUTPUT);
  pinMode(LEFT_1_PIN, OUTPUT);
  pinMode(LEFT_2_PIN, OUTPUT);
  pinMode(RIGHT_PWM_PIN, OUTPUT);
  pinMode(RIGHT_1_PIN, OUTPUT);
  pinMode(RIGHT_2_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
_speedL = 0;
_speedR = 0;
  Robot::speed(0,0);  
  Robot::stop();
}

void Robot::speed(int speedL, int speedR) {
_speedL = speedL;
_speedR = speedR;
Robot::updatePWM();
}

void Robot::updatePWM(void) {

unsigned long currentMicro = micros();
if (currentMicro - _previousMicros >= _intervalMicros) {
_previousMicros = currentMicro;
 Robot::pwm(RIGHT_PWM_PIN, _speedR);
 Robot::pwm(LEFT_PWM_PIN, _speedL);
  }
}

void Robot::pwm(int pin, int del){
 int spd = map(del,0, 100, 0, 16000); //set speed percent to a PWM from 0 to 16000
 digitalWrite(pin, HIGH);
 delayMicroseconds(spd);
 digitalWrite(pin, LOW);
 delayMicroseconds(16000 - spd);
}

int Robot::ping() {
  
digitalWrite(TRIGGER_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIGGER_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIGGER_PIN, LOW);

_duration = pulseIn(ECHO_PIN, HIGH);
_distance= _duration*0.034/2;
if (_distance > 1000) { _distance = 0; }
return _distance;
}

void Robot::forward(int onTime) {
  //Forward Motion
  digitalWrite(LEFT_1_PIN, LOW);
  digitalWrite(LEFT_2_PIN, HIGH);
  digitalWrite(RIGHT_1_PIN, LOW);
  digitalWrite(RIGHT_2_PIN, HIGH);
_onTime = onTime;
_isrunning = 1;
}

void Robot::backward(int onTime) {
  //Backward Motion
  digitalWrite(LEFT_1_PIN, HIGH);
  digitalWrite(LEFT_2_PIN, LOW);
  digitalWrite(RIGHT_1_PIN, HIGH);
  digitalWrite(RIGHT_2_PIN, LOW);
_onTime = onTime;
_isrunning = 1;
}

void Robot::turnLeft(int onTime) {
  
  digitalWrite(LEFT_1_PIN, LOW);
  digitalWrite(LEFT_2_PIN, HIGH);
  digitalWrite(RIGHT_1_PIN, HIGH);
  digitalWrite(RIGHT_2_PIN, LOW);
_onTime = onTime;
_isrunning = 1;
}

void Robot::turnRight(int onTime) {
  
  digitalWrite(LEFT_1_PIN, HIGH);
  digitalWrite(LEFT_2_PIN, LOW);
  digitalWrite(RIGHT_1_PIN, LOW);
  digitalWrite(RIGHT_2_PIN, HIGH);
_onTime = onTime;
_isrunning = 1;
}

void Robot::stop() {
  
  digitalWrite(LEFT_1_PIN, LOW);
  digitalWrite(LEFT_2_PIN, LOW);
  digitalWrite(RIGHT_1_PIN, LOW);
  digitalWrite(RIGHT_2_PIN, LOW);
_isrunning = 0;
}

void Robot::update() {
	unsigned long currentMillis = millis();
	if (currentMillis - _previousMillis >= _onTime) {
		_previousMillis = currentMillis;
		Robot::stop();
		Robot::speed(0, 0);
	}
Robot::updatePWM();
}

int Robot::isRunning() {
	return (_isrunning);

};
