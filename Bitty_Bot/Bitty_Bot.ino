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
const int MaxSpeed = 255;
const int CorrectLeft = 0;
const int CorrectRight = 0;
int speed;

void setup()
{

pinMode (L1Pin, OUTPUT);
pinMode (L2Pin, OUTPUT);
pinMode (L3Pin, OUTPUT);
pinMode (L4Pin, OUTPUT);
pinMode (PWMLPin, OUTPUT);
pinMode (PWMRPin, OUTPUT);


}

void loop()
{

speed = 100;
left();
delay(500);
right();
delay(500);
forward();
delay(500);
backward();
delay(500);
allstop();
//delay (2500);

speed = 75;
circleleft(100);
allstop();
//delay(100);
speed = 125;
circleright(100);
allstop();
//delay(100);
speed = 60;
left();
delay(200);
allstop();
//delay(15000); //long delay

}

void left() {
	digitalWrite (L1Pin, 1);
	digitalWrite (L2Pin, 0);
	analogWrite (PWMLPin, speed);
	digitalWrite (L3Pin, 0);
	digitalWrite (L4Pin, 1);
	analogWrite (PWMRPin, speed);
}

int right() {
	digitalWrite(L1Pin, 0);
	digitalWrite(L2Pin, 1);
	analogWrite (PWMLPin, speed);
	digitalWrite(L3Pin, 1);
	digitalWrite(L4Pin, 0);
	analogWrite(PWMRPin, speed);
}

int forward() {
	digitalWrite(L1Pin, 1);
	digitalWrite(L2Pin, 0);
	digitalWrite(L3Pin, 1);
	digitalWrite(L4Pin, 0);
	analogWrite(PWMLPin, speed);
	analogWrite(PWMRPin, speed);
}

int backward() {
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