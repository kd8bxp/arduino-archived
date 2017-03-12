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
 
//Added a Drop Sensor function, this can be used to make sure the robot doesn't fall off a table or down stairs Aug 31, 2015
//irdrop();

//Digital Pins for IR sensor
const int IRPIN1 = 51; //Pin for Left IR Sensor
const int IRPIN2 = 53; //Pin for Center IR Sensor
const int IRPIN3 = 49; //Pin for Right IR Sensor

//Analog Pins for IR Sensor
const int IRaPIN1 = A8; //Left IR Sensor
const int IRaPIN2 = A9; //Center IR Sensor
const int IRaPIN3 = A10; //Right IR Sensor

void setup()
{

pinMode (L1Pin, OUTPUT);
pinMode (L2Pin, OUTPUT);
pinMode (L3Pin, OUTPUT);
pinMode (L4Pin, OUTPUT);
pinMode (PWMLPin, OUTPUT);
pinMode (PWMRPin, OUTPUT);

pinMode (IRPIN1, INPUT);
pinMode (IRPIN2, INPUT);
pinMode (IRPIN3, INPUT);

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

//This small section of code shows the relationship of speed vs time 
irdrop();
speed = 65; // speed is set low = slow  
forward(); // go forward 
delay(100); // keep going forward 


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

if (temp <= 7.5 || temp >=7.1) {
	Serial.println("Very Low Voltage");
	tone(9, 1000, 100);
}

if (temp <= 7.00) {
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

void irdrop() {
	int ir1 = digitalRead(IRPIN1);
	int ir2 = digitalRead(IRPIN2);
	int ir3 = digitalRead(IRPIN3);
	Serial.print("IR1: PIN 51: ");
	Serial.println(ir1);
	Serial.print("IR2: PIN 53: ");
	Serial.println(ir2);
	Serial.print("IR3: PIN 49: ");
	Serial.println(ir3);
	
	if (ir1 == 1 || ir2 == 1 || ir3 == 1) { 
		allstop();
		delay(500);
		speed=150;
		backward();
		delay(700);
		allstop();
		righttight();
		delay(700);
		allstop();
	}
}
