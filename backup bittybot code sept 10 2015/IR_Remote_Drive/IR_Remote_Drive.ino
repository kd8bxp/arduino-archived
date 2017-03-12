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

//Added IR Remote Control Sep 5 2015 - LeRoy Miller

//IR Reciever is hooked to Pin 24 - this may or may not change

#include "IRremote.h"

int receiver = 24;
IRrecv irrecv(receiver);
decode_results results;

/* CODES For My Remote - Yours probably will be different (unless you are using a Make:IT Robotics Remote IR Controller that looks like a "Wing")

up    2519209162 -HEX 0x962814CA FORWARD
down  1502638218 -HEX 0x5990708A BACKWARD
left  2999730842 -HEX 0xB2CC429A LEFT TIGHT
right 2953994586 -HEX 0xB012615A RIGHT TIGHT

sw7 2054033594 - HEX 0x7A6E10BA  ALLSTOP
sw8 2534555274 - HEX 0x97123E8A  Not Yet Used
sw5 2368359343 - HEX 0x8D2A4BAF  Not Yet Used
sw6 472047109  - HEX 0x1C22DE05  Not Yet Used This code looks too short (if so HEX will be wrong too)
*/


void setup()
{
Serial.begin(9600);
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

irrecv.enableIRIn(); //start the receiver

}

void loop()
{

display.setTextColor(BLACK);
display.setCursor(0,46);
display.print("Volts: ");
display.print(volts);
display.print(" V");
volts = checkvoltage();
display.setTextColor(WHITE);
display.setCursor(0,46);
display.print("Volts: ");
display.print(volts);
display.print(" V");
display.display();
allstop();

if (irrecv.decode(&results)) {
	translateIR();
	irrecv.resume();
}
}

void translateIR() {

switch(results.value) {
	
case 0x962814CA:
//up = forward
forward();
delay(100);
Serial.println("Forward");
break;

case 0x5990708A:
//down = backward
backward();
delay(100);
Serial.println("Backward");
break;

case 0xB012615A:
//right = tight right turn
righttight();
Serial.println("TightRight");
delay(100);
break;

case 0xB2CC429A:
//left = tight left turn
lefttight();
delay(100);
Serial.println("TightLeft");
break;

case 0x7A6E10BA:
//sw7 - All Stop
allstop();
delay(500);
break;

/*case 16197847:
//Yellow Button left backup turn
leftb();
delay(50);
break;

case 16214167:
//purplish/pink right backup turn
rightb();
delay(50);
break;

case 3556612737:
//red not marked - left turn (not tight)
left();
delay(50);
break;

case 16208047:
//blue dark not marked - right turn (not tight)
right();
delay(50);
break;

default:
break;
*/

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
delay(1000);

if (temp <= 7.5 || temp >=6.3) {
	Serial.println("Very Low Voltage");
	tone(9, 1000, 100);
}

if (temp <= 6.3) {
	Serial.println("Voltage Crital");
	sos();
}
return (temp);
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

