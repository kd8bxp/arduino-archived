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

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 display(8);

byte DTMFread;            // The DTMFread variable will be used to interpret the output of the DTMF module.
const int STQ = 3;        // Attach DTMF Module STQ Pin to Arduino Digital Pin 3
 const int Q4 = 4;        // Attach DTMF Module Q4  Pin to Arduino Digital Pin 4
 const int Q3 = 5;        // Attach DTMF Module Q3  Pin to Arduino Digital Pin 5
 const int Q2 = 6;        // Attach DTMF Module Q2  Pin to Arduino Digital Pin 6
 const int Q1 = 7;        // Attach DTMF Module Q1  Pin to Arduino Digital Pin 7

int leftspeed = 75;
int rightspeed = 75;

int botarray[10];
int ButtonPin = 2;

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
  pinMode(ButtonPin, INPUT);
  Serial.println("Hello");
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println(" Bitty-Bot  Robot");
display.println("Startup...");
display.display();
delay(2000);
//display.clearDisplay();
display.setCursor(0,8);
display.setTextColor(BLACK);
display.println("Startup...");
display.setCursor(0,8);
display.setTextColor(WHITE);
display.println("Ready....");
display.display();
delay(2000);
display.clearDisplay();
display.display();
  programbot();
}

void loop() {
	
if (digitalRead(ButtonPin) == LOW) {	

for (int steps=0; steps < 10; steps++) {
	//Check Tone and set action
	display.setCursor(0,0);
	display.println("Run Mode:");
	display.display();
if (botarray[steps] == 2) {bot.forward(250,leftspeed,rightspeed);} //2 is forward
if (botarray[steps] == 5) {bot.stop();}						   //5 is Stop
if (botarray[steps] == 8) {bot.back(250, leftspeed, rightspeed);}  //8 is resverse
if (botarray[steps] == 4) {bot.left(250, leftspeed, rightspeed+20);} //4 turn left
if (botarray[steps] == 6) {bot.right(250, leftspeed+20, rightspeed);} //6 turn right
if (botarray[steps] == 11) {lessspeed();} // 11 is the number returned with the * key used to decrease the speed of rover
if (botarray[steps] == 12) {morespeed();} //12 is the number returned with the # key used to increase the speed of the rover

bot.update();
delay(750);
		}
	}
}

void programbot() {
	display.setCursor(0,0);
	display.setTextColor(WHITE);
	display.println("Program Mode");
	display.setCursor(0,18);
	display.println("'D' Exits");
	display.display();
	for (int steps=0; steps < 10; steps++) {
		display.setCursor(0,9);
		display.println("Step: ");
		display.setCursor(35, 9);
		display.setTextColor(BLACK);
		display.println(steps-1);
		display.setCursor(35, 9);
		display.setTextColor(WHITE);
		display.println(steps);
		display.display();
		DTMFread = DTMFtone();
		botarray[steps] = DTMFread;
		if (DTMFread == 15) {return;}
		delay(250);
	}
	display.clearDisplay();
	display.display();
}


void morespeed() {
	
	leftspeed = leftspeed + 5;
	rightspeed = rightspeed +5;
	if (leftspeed > 225) {leftspeed = 225;}
	if (rightspeed > 255) {rightspeed = 225;}
}

void lessspeed() {
	leftspeed = leftspeed - 5;
	rightspeed = rightspeed - 5;
	if (leftspeed < 60) {leftspeed = 60;}
	if (rightspeed < 60) {rightspeed = 60;}
}

int DTMFtone() {

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
 
return DTMFread;
	 } 
	 DTMFtone();
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

/* Personal notes: May 4 2016 - Moved DTMF tone reading to subroutine, and added code for setting speed using DTMF tones */