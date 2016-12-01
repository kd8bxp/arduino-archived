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

//Added IR Remote Control Sep 5 2015 - LeRoy Miller

//IR Reciever is hooked to Pin 11 - this may or may not change

#include "IRremote.h"

int receiver = 24;
IRrecv irrecv(receiver);
decode_results results;

/* CODES For My Remote - Yours probably will be different
1685187709 (Green Button Not marked) - UP
343449017 (light blue button not marked) - OK 
1097729501 (darker blue button not marked) - Down
1041374241 (dark purple button not marked) - right
2214874045 (orange button not marked) - left
247 - seems like it might be a button change code
16240687 (may also be a button change code - or delay of same button pushed code)
3571254145 - off
3457774333 - on
16236607 (Not sure??)
16197847 (Yellow Button) - left backup
16214167 (Purplish/pink button) - right backup
3556612737 (red button not marked) - left turn (not tight)
16208047 (blue darkish not marked) - right turn (not tight)
*/


void setup()
{

pinMode (L1Pin, OUTPUT);
pinMode (L2Pin, OUTPUT);
pinMode (L3Pin, OUTPUT);
pinMode (L4Pin, OUTPUT);
pinMode (PWMLPin, OUTPUT);
pinMode (PWMRPin, OUTPUT);

irrecv.enableIRIn(); //start the receiver

}

void loop()
{

if (irrecv.decode(&results)) {
	translateIR();
	irrecv.resume();
}
}

void translateIR() {

switch(results.value) {
case 1685187709:
//up = forward
forward();
delay(100);
break;

case 1097729501:
//down = backward
backward();
delay(100);
break;

case 1041374241:
//right = tight right turn
righttight();
delay(100);
break;

case 2214874045:
//left = tight left turn
lefttight();
delay(100);
break;

case 343449017:
//ok
break;

case 16197847:
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