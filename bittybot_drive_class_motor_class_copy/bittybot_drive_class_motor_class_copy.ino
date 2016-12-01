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
    
public: 
Motor(int enableL, int enableR, int Left1, int Left2, int Right1, int Right2) {
	_enableL = enableL;
	_enableR = enableR;
	_Left1 = Left1;
	_Left2 = Left2;
	_Right1 = Right1;
	_Right2 = Right2;
	_previousMillis = 0;
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

void forward(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	_OnTime = OnTime;
	_isrunning = 1;
}

void back(int OnTime) {
	digitalWrite(_Left1, HIGH);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, HIGH);
	digitalWrite(_Right2, LOW);
	_OnTime = OnTime;
	_isrunning = 1;

}

void rightTight(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, HIGH);
	digitalWrite(_Right2, LOW);
	_OnTime = OnTime;
	_isrunning = 1;
}

void leftTight(int OnTime) {
	digitalWrite(_Left1, HIGH);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	_OnTime = OnTime;
	_isrunning = 1;
}

void right(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);
	analogWrite(_enableR, 0);
	_OnTime = OnTime;
	_isrunning = 1;
}

void left(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	analogWrite(_enableL, 0);
	_OnTime = OnTime;
	_isrunning = 1;
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
//We need to know where the magnets are at the start, that is what this is suppose to do, give a common starting point.
	Speed(50,50);
		while (digitalRead(3) != HIGH) {
		
		right(1000);
			}
			stop();
			Speed(50,50);
		while (digitalRead(2) != HIGH) {
		
		left(1000);
	}
	stop();
	Speed(0,0);
}

int IsRunning() {
	return (_isrunning);
}
};

int _leftEncoderPin = 2;
	int _rightEncoderPin = 3;
	unsigned int _leftrpm;
	unsigned int _rightrpm;
	volatile byte _leftpulses;
	volatile byte _rightpulses;
	unsigned long _encoderTimeOld;
	unsigned int _pulsesperturn = 1;

int leftspeed = 75;
int rightspeed = 75;



Motor bot(44,46,36,38,40,42); //Left Enable, Right Enable, Pin1 for Left, Pin2 for Left, Pin 1 for Right, Pin 2 for Right

void updaterpm() {
	unsigned long currentMillis = millis();
int tleft = leftspeed;
int tright = rightspeed;
if (currentMillis - _encoderTimeOld >=1000); {
if (_leftpulses > _rightpulses) {tleft = leftspeed - 5;}
if (_leftpulses < _rightpulses) {tleft = leftspeed + 5;}
if (_rightpulses > _leftpulses) {tright = rightspeed - 5;}
if (_rightpulses < _leftpulses) {tright = rightspeed + 5;}

if (tleft >= 255 || tleft <= 0) {tleft = leftspeed;}
if (tright >= 255 || tright <=0) {tright = rightspeed;}
//bot.Speed(0,0);
bot.Speed(tleft, tright);
//analogWrite(44, rightspeed);
//analogWrite(46, leftspeed);

_rightpulses = 0;
_leftpulses = 0;
_encoderTimeOld = currentMillis;
}

	//if (currentMillis - _encoderTimeOld >= 1000) {
//		Serial.print("Pulses = L: ");
//		Serial.print(_leftpulses);
//		Serial.print("  R: ");
//		Serial.println(_rightpulses);
//		_rightrpm = (60 * 1000 / _pulsesperturn)/(currentMillis - _encoderTimeOld) * _rightpulses;
//		_leftrpm = (60 * 1000 / _pulsesperturn)/(currentMillis - _encoderTimeOld) * _leftpulses;
//		_encoderTimeOld=currentMillis;
//		_rightpulses = 0;
//		_leftpulses = 0;
//		Serial.print("RPM = L: ");
//		Serial.print(_leftrpm);
//		Serial.print("  R: ");
//		Serial.println(_rightrpm);
	//}
}

void leftcounter() {
	_leftpulses++;
}

void rightcounter() {
	_rightpulses++;
}

void setup () {
	Serial.begin(9600);
	bot.begin();

	pinMode(_leftEncoderPin, INPUT);
	pinMode(_rightEncoderPin, INPUT);
	attachInterrupt(0, leftcounter, RISING); //left wheel encoder
	attachInterrupt(1, rightcounter, RISING); //right wheel encoder
	
	_leftpulses = 0;
	_rightpulses =0;
	_leftrpm = 0;
	_rightrpm = 0;
	_encoderTimeOld = 0;
	
	//bot.Speed(leftspeed,rightspeed);
	//bot.forward(15000);

	
	Serial.println("Hello!");
}

void loop() {
	bot.Speed(leftspeed,rightspeed);
	bot.forward(15000);
	
	while (bot.IsRunning()) {
		
		Serial.println(bot.IsRunning());
		bot.update();
		updaterpm();
	}
	
	bot.Speed(leftspeed,rightspeed);
	bot.back(15000);
	
	while (bot.IsRunning()) {
		Serial.println("Bot Running");
		bot.update();
		updaterpm();
		}
}