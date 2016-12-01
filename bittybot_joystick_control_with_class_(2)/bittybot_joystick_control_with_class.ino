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

#include <NewPing.h>
#include <VirtualWire.h>

	int leftEncoderPin = 2;
	int rightEncoderPin = 3;
	unsigned int leftrpm;
	unsigned int rightrpm;
	volatile byte leftpulses;
	volatile byte rightpulses;
	unsigned long encoderTimeOld;
	unsigned int pulsesperturn = 1;

int leftspeedmax  = 175;
int rightspeedmax = 175;
int leftspeed;
int rightspeed;

#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MAX_DISTANCE 200

byte xval, yval;
signed int x,y;
char hex[9] = {0};

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//IF you are using a hall effect sensor for your wheel encoder, it's a good idea to start the wheels at the same point, and use calibrate
//otherwise set calibrate to zero.
Motor bot(44,46,36,38,40,42, 1); //Left Enable, Right Enable, Pin1 for Left, Pin2 for Left, Pin 1 for Right, Pin 2 for Right, use calibrate (1 or 0)

void updaterpm() {
	unsigned long currentMillis = millis();
int tleft = leftspeed;
int tright = rightspeed;
if (currentMillis - encoderTimeOld >=1000); {
if (leftpulses > rightpulses) {tleft = leftspeed - 5;}
if (leftpulses < rightpulses) {tleft = leftspeed + 5;}
if (rightpulses > leftpulses) {tright = rightspeed - 5;}
if (rightpulses < leftpulses) {tright = rightspeed + 5;}

if (tleft >= 255 || tleft <= 0) {tleft = leftspeed;}
if (tright >= 255 || tright <=0) {tright = rightspeed;}

bot.Speed(tleft, tright);

rightpulses = 0;
leftpulses = 0;
encoderTimeOld = currentMillis;
	}
}

void leftcounter() {
	leftpulses++;
}

void rightcounter() {
	rightpulses++;
}

void setup () {
	Serial.begin(9600);
	bot.begin();

	pinMode(leftEncoderPin, INPUT);
	pinMode(rightEncoderPin, INPUT);
	attachInterrupt(0, leftcounter, RISING); //left wheel encoder
	attachInterrupt(1, rightcounter, RISING); //right wheel encoder
	
	leftpulses = 0;
	rightpulses =0;
	leftrpm = 0;
	rightrpm = 0;
	encoderTimeOld = 0;
	
	//bot.Speed(leftspeed,rightspeed);
	//bot.forward(15000);

	vw_set_rx_pin(22);
	vw_setup(2000);
	vw_rx_start();
	
	Serial.println("Hello!");

//delay(10000); //just wait for you to sit the robot down 

}

void checksonar() {
	unsigned int uS = sonar.ping();
	unsigned int sI = (uS/ US_ROUNDTRIP_IN);
	if (sI > 20 && sI <=29) {
		leftspeed = 100;
		rightspeed = 100;
		bot.Speed(leftspeed,rightspeed);
	}
	if (sI > 3 && sI <=19) {
		leftspeed = 75;
		rightspeed = 75;
		bot.Speed(leftspeed,rightspeed);
	}
	if (sI <= 2) {
		bot.stop();
		leftspeed = leftspeedmax;
		rightspeed = rightspeedmax;
		
		bot.leftTight(700, leftspeed, rightspeed);
			while (bot.IsRunning()) {
			Serial.println("Bot Running");
			bot.update();
			updaterpm();
		}
		bot.stop();
	}
}

void loop() {
	
	
	uint8_t buf[VW_MAX_MESSAGE_LEN];
	uint8_t buflen = VW_MAX_MESSAGE_LEN;
	
	if (vw_get_message(buf, &buflen)) {
		int i;
		for (i = 0; i<buflen; i++) {
			hex[i]=buf[i];
		}
	}
	
	sscanf(hex, "%02X%02X %02d%02d", &xval, &yval, &x, &y);
	
	if (x == 0 && y == 0) {
		bot.Speed(0,0);
		bot.stop();
	}
	
	if (x == 1 && y == 0) {
		bot.right(100,xval,xval);
		}

	if (x == -1 && y == 0) {
		bot.left(100, xval,xval);
	}
	
	if (x == 0 && y == -1) {
		bot.back(100,yval,yval);
	}
	
	if (x == 0 && y == 1) {
		bot.forward(100,yval,yval);
	}
	
	if (x == 1 && y == 1) {
		bot.forward(100,yval,xval);
	}
	
	if (x == -1 && y == 1) {
		bot.forward(100, xval, yval);
	}
	
	if (x == -1 && y == -1) {
		bot.back(100,xval,yval);
	}
	
	if (x == 1 && y == -1) {
		bot.back(100,yval,xval);
	}
}