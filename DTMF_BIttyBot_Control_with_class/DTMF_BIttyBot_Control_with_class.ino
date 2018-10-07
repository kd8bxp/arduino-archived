//BittyBot Rover DTMF Drive with class motor control by LeRoy Miller May 4, 2016
//see credits at end of file for parts of code based on MT8870 DTMF Servo by Scott C. Aug 4th 2015 

/* Copyright 2016 LeRoy Miller
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses
 */

class Motor {

	int _enableL;
	int _enableR;
	int _Left1; //L1 
	int _Left2; //L2
	int _Right1; //L3
	int _Right2; //L4
	unsigned long _previousMillis;
	long _OnTime;
	int _speedL;
	int _speedR;
    int _isrunning;
    int _useCalibrate; //1 = use the encoder calibrate, 0 = do not use calibrate encoder
    
public: 
Motor(int enableL, int enableR, int Left1, int Left2, int Right1, int Right2, int cal) {
	_enableL = enableL;
	_enableR = enableR;
	_Left1 = Left1;
	_Left2 = Left2;
	_Right1 = Right1;
	_Right2 = Right2;
	_previousMillis = 0;
	_useCalibrate = cal;
}

void begin() {
	pinMode(_enableL, OUTPUT);
	pinMode(_enableR, OUTPUT);
	pinMode(_Left1, OUTPUT);
	pinMode(_Left2, OUTPUT);
	pinMode(_Right1, OUTPUT);
	pinMode(_Right2, OUTPUT);
	calibrate();
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);
	Speed(0,0);
	

}
 
void Speed(int speedL, int speedR) {
	_speedL = speedL;
	_speedR = speedR;
	//digitalWrite(_Left1, LOW);
	//digitalWrite(_Left2, LOW);
	//digitalWrite(_Right1, LOW);
	//digitalWrite(_Right2, LOW);
	analogWrite(_enableL, _speedL);
	analogWrite(_enableR, _speedR);

}

void stop() {

	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);
	Speed(0,0);
	_isrunning = 0;
}

void ramp() {
	Speed(255,255);
}

void forward(int OnTime, int speedL, int speedR) {
	ramp();
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	_OnTime = OnTime;
	_isrunning = 1;
	Speed(speedL, speedR);

}

void back(int OnTime, int speedL, int speedR) {
	ramp();
	digitalWrite(_Left1, HIGH);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, HIGH);
	digitalWrite(_Right2, LOW);
	_OnTime = OnTime;
	_isrunning = 1;
	Speed(speedL, speedR);
}

void rightTight(int OnTime,int speedL, int speedR) {
	ramp();
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, HIGH);
	digitalWrite(_Right2, LOW);
	_OnTime = OnTime;
	_isrunning = 1;
	Speed(speedL, speedR);
}

void leftTight(int OnTime,int speedL, int speedR) {
	ramp();
	digitalWrite(_Left1, HIGH);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	_OnTime = OnTime;
	_isrunning = 1;
	Speed(speedL, speedR);
}

void right(int OnTime, int speedL, int speedR) {
	Speed(255,0);
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);
	analogWrite(_enableR, 0);
	_OnTime = OnTime;
	_isrunning = 1;
	Speed(speedL, 0);
}

void left(int OnTime,int speedL, int speedR) {
	Speed(0,255);
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	analogWrite(_enableL, 0);
	_OnTime = OnTime;
	_isrunning = 1;
	Speed(0,speedR);
}

void update() {
	unsigned long currentMillis = millis();
	if (currentMillis - _previousMillis >= _OnTime) {
		_previousMillis = currentMillis;
		stop();
		Speed(_speedL, _speedR);
	}
}

void calibrate() {
	
	if (_useCalibrate == 1) {
//We need to know where the magnets are at the start, that is what this is suppose to do, give a common starting point.
	//Speed(50,50);
		while (digitalRead(3) != HIGH) {
		
		right(1000, 50,50);
			}
			stop();
			//Speed(50,50);
		while (digitalRead(2) != HIGH) {
		
		left(1000,50,50);
	}
	stop();
	Speed(0,0);
	}
}

int IsRunning() {
	return (_isrunning);
}
};



byte DTMFread;            // The DTMFread variable will be used to interpret the output of the DTMF module.
const int STQ = 3;        // Attach DTMF Module STQ Pin to Arduino Digital Pin 3
 const int Q4 = 4;        // Attach DTMF Module Q4  Pin to Arduino Digital Pin 4
 const int Q3 = 5;        // Attach DTMF Module Q3  Pin to Arduino Digital Pin 5
 const int Q2 = 6;        // Attach DTMF Module Q2  Pin to Arduino Digital Pin 6
 const int Q1 = 7;        // Attach DTMF Module Q1  Pin to Arduino Digital Pin 7

int leftspeed = 75;
int rightspeed = 75;


//IF you are using a hall effect sensor for your wheel encoder, it's a good idea to start the wheels at the same point, and use calibrate
//otherwise set calibrate to zero.
Motor bot(44,46,36,38,40,42, 0); //Left Enable, Right Enable, Pin1 for Left, Pin2 for Left, Pin 1 for Right, Pin 2 for Right, use calibrate (1 or 0)



void setup() {
  
  Serial.begin(9600);
  bot.begin();
  //Setup the INPUT pins on the Arduino
  pinMode(STQ, INPUT);
  pinMode(Q4, INPUT);
  pinMode(Q3, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q1, INPUT);
  Serial.println("Hello");
  
}

void loop() {
	
  if(digitalRead(STQ)==HIGH){       //When a DTMF tone is detected, STQ will read HIGH for the duration of the tone.
    DTMFread=0;
    if(digitalRead(Q1)==HIGH){      //If Q1 reads HIGH, then add 1 to the DTMFread variable
      DTMFread=DTMFread+1;
    }
    if(digitalRead(Q2)==HIGH){      //If Q2 reads HIGH, then add 2 to the DTMFread variable
      DTMFread=DTMFread+2;
    }
    if(digitalRead(Q3)==HIGH){      //If Q3 reads HIGH, then add 4 to the DTMFread variable
      DTMFread=DTMFread+4;
    }
    if(digitalRead(Q4)==HIGH){      //If Q4 reads HIGH, then add 8 to the DTMFread variable
      DTMFread=DTMFread+8;
    }
    Serial.println(DTMFread);
//Check Tone and set action
if (DTMFread == 2) {bot.forward(100,leftspeed,rightspeed);} //2 is forward
if (DTMFread == 5) {bot.stop();}						   //5 is Stop
if (DTMFread == 8) {bot.back(100, leftspeed, rightspeed);}  //8 is resverse
if (DTMFread == 4) {bot.left(100, leftspeed, rightspeed+20);} //4 turn left
if (DTMFread == 6) {bot.right(100, leftspeed+20, rightspeed);} //6 turn right
 bot.update();  
      }
bot.update();
}


/* ==================================================================================================================================================
         Project: MT8870 DTMF Servo sketch
          Author: Scott C
         Created: 4th August 2015
     Arduino IDE: 1.6.4
         Website: http://arduinobasics.blogspot.com/p/arduino-basics-projects-page.html
     Description: This project will allow you to control a Servo motor using an Arduino UNO and a MT8870 DTMF Module.
                  The DTMF signal is received through the 3.5mm port of the DTMF module and is decoded. We will use the
                  decoded output to control the position of the Servo. A SG-5010 Servo motor was used in this project.
===================================================================================================================================================== */