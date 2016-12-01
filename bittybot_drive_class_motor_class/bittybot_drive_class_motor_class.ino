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
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);
}
 
void Speed(int speedL, int speedR) {
	_speedL = speedL;
	_speedR = speedR;
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);
	analogWrite(_enableL, _speedL);
	analogWrite(_enableR, _speedR);

}

void stop() {

	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);

}

void forward(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	_OnTime = OnTime;
}

void back(int OnTime) {
	digitalWrite(_Left1, HIGH);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, HIGH);
	digitalWrite(_Right2, LOW);
	_OnTime = OnTime;
}

void rightTight(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, HIGH);
	digitalWrite(_Right2, LOW);
	_OnTime = OnTime;
}

void leftTight(int OnTime) {
	digitalWrite(_Left1, HIGH);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	_OnTime = OnTime;
}

void right(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, HIGH);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, LOW);
	analogWrite(_enableR, 0);
	_OnTime = OnTime;
}

void left(int OnTime) {
	digitalWrite(_Left1, LOW);
	digitalWrite(_Left2, LOW);
	digitalWrite(_Right1, LOW);
	digitalWrite(_Right2, HIGH);
	analogWrite(_enableL, 0);
	_OnTime = OnTime;
}

void update() {
	unsigned long currentMillis = millis();
	if (currentMillis - _previousMillis >= _OnTime) {
		_previousMillis = currentMillis;
		stop();
		Speed(_speedL, _speedR);
	}
}
};


Motor bot(44,46,36,38,40,42); //Left Enable, Right Enable, Pin1 for Left, Pin2 for Left, Pin 1 for Right, Pin 2 for Right

void setup () {
	Serial.begin(9600);
	bot.begin();
	
		
	bot.Speed(75,75);
	bot.forward(15000);

	
	Serial.println("Hello!");
}

void loop() {
	bot.update();
}