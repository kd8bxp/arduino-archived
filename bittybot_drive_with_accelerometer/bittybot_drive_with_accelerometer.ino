// Motor Controls Link to Original Article
// Automation Technology Club
// Motor Controls
// Free to use as you see if - Pin Numbering by Grand Poobah
// Subroutines Aug 18 - LeRoy Miller
// additions will be added for speed correction of wheels

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

//Sept 8 - Added Voltage Sensor with Shut off at 6.2volts

int val11; //for voltage sensor
int val2; //for voltage sensor
float volts; //for voltage sensor

//Sept 8 - Added OLED Screen for Display information
//Needed for OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
 
//Added Sep 20 for Accelerometer - The Accelerometer I used is a GY-61 ADXL335 3-Axis Analog module (X, Y, Z) 
//All three axis are hooked up, but for this demo only X & Y are being used.

int x, y, z;
int xcal, ycal, zcal;

int xPin = A11;
int yPin = A12;
int zPin = A13;
//Two need functions for the accelerometer.
//calibrateaccelerometer() - robot should not be moving while calibarting, this just get a quick static reading of it at rest
//readaccelerometer() - compares the at rest reading + or - two, this can be used if the robot stops moving but should be moving.
int moveflag = 0; //a flag to tell if the motors should be moving (not used in this demo, but maybe added later)

//added Sep 20 - Message Update for Display
String message="";
//messageclear() - Clear old message from display
//messagedisplay() - display new message - messages should be short

void setup()
{

pinMode (L1Pin, OUTPUT);
pinMode (L2Pin, OUTPUT);
pinMode (L3Pin, OUTPUT);
pinMode (L4Pin, OUTPUT);
pinMode (PWMLPin, OUTPUT);
pinMode (PWMRPin, OUTPUT);

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
Serial.begin(9600);

display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println(" Bitty-Bot  Robot");
display.display();

calibrateaccelerometer();

}

void loop()
{
speed = 45;
//forward();

display.setTextColor(WHITE);
display.setCursor(0,56); //was (0, 46)
display.print("Volts: ");
display.setTextColor(BLACK);
display.print(volts);
display.setTextColor(WHITE);
display.print(" V");
display.display();
//volts = checkvoltage();
checkvoltage();
display.setTextColor(WHITE);
display.setCursor(0,56);
display.print("Volts: ");
display.print(volts);
display.print(" V");
display.display();

display.setTextColor(BLACK);
display.setCursor(0, 10);
display.print("X: ");
display.print(x);
display.print(" Y: ");
display.print(y);
display.print(" Z: ");
display.print(z);
display.display();
readaccelerometer();
delay(100);
display.setTextColor(WHITE);
display.setCursor(0, 10);
display.print("X: ");
display.print(x);
display.print(" Y: ");
display.print(y);
display.print(" Z: ");
display.print(z);
display.display();


delay(1000); //wait

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

int checkvoltage() {
float temp;

val11=analogRead(1);
temp=(val11/4.092)/10;
val11=(int)temp * 10;//
val2=((val11%100)/10);

Serial.print("Raw Reading: ");
Serial.println(temp);
Serial.println("Corrected Reading: ");
Serial.println(val2);
//delay(1000);

if (temp <= 7.5 && temp >=6.3) {
	Serial.println("Very Low Voltage");
	tone(9, 1000, 100);
}

if (temp <= 6.3) {
	Serial.println("Voltage Crital");
	sos();
}
volts = temp;

}

int sos() {
	
	for (int xx=0; xx<3; xx++) {
		tone(9, 440, 100);
		delay(200);
		noTone(9);
	}
	for (int xx=0; xx<3; xx++){
		tone(9, 440, 300);
		delay(400);
		noTone(9);
	}
	for (int xx=0; xx<3; xx++) {
		tone(9, 440, 100);
		delay(200);
		noTone(9);
	}
	delay(1000);
	sos();
}

void calibrateaccelerometer() {
  xcal = analogRead(xPin);       // read analog input pin 0
  ycal = analogRead(yPin);       // read analog input pin 1
  zcal = analogRead(zPin);       // read analog input pin 1
  
}

void readaccelerometer() {
	x = analogRead(xPin);
	y = analogRead(yPin);
	z = analogRead(zPin);

/*	
	if (x == xcal || x <= xcal-1 || x >= xcal+1) {
	//if (x == xcal) {
		Serial.println("X Not Moving....");
		messageclear();
		message = "Not Moving";
		messagedisplay();
		allstop();
		rightb();
		delay(1000);
		allstop();
	}
	
	if (x < xcal-1) {
		Serial.println("X Moving Backward");
		messageclear();
		message = "Backward";
		messagedisplay();
		//backward();
	}
	
	if (x > xcal+1) {
		Serial.println("X Moving Forward!");
		messageclear();
		message = "Forward";
		messagedisplay();
		//forward();
	}
*/	
	if (y == ycal || y <= ycal -2 || y >= ycal+2) {
		Serial.println("Y Not moving...");
		messageclear();
		message = "Not Moving";
		messagedisplay();
		//allstop();
		//backward();
		//delay(500);
	}
	
	if (y < ycal-2) {Serial.println("Y Jar Right");
	messageclear();
	message = "Right";
	messagedisplay();
	}
	if (y > ycal+2) {Serial.println("Y Jar Left");
	messageclear();
	message = "Left";
	messagedisplay();
	}
}

void messageclear() {
display.setTextColor(BLACK);
display.setCursor(0,19);
display.print(message);
display.display();
}

void messagedisplay() {
display.setTextColor(WHITE);
display.setCursor(0,19);
display.print(message);
display.display();
}