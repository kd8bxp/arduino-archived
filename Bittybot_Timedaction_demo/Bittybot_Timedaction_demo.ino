    #include <TimedAction.h>
    #include <NewPing.h>
  
    //this initializes a TimedAction object that will change the state of an LED every second.
  
     //this initializes a TimedAction object that will change the state of an LED
    //according to the serial buffer contents, every 50 milliseconds
    TimedAction LedBlinkAction = TimedAction(300, blink);
    TimedAction physicalPixelAction =       TimedAction(1000,physicalPixel);
    TimedAction blink11action = TimedAction(500, blink11);
    TimedAction blink10action = TimedAction(250, blink10);
    TimedAction blink9action = TimedAction(100, blink9);
    TimedAction blink8action = TimedAction(750, blink8);
    TimedAction pingAction = TimedAction(15, pingdo);
    TimedAction StopMotorAction = TimedAction(0, stop);
    //pin / state variables
    #define ledPin 13
    #define physicalPin 12
    #define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
	#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
	#define MAX_DISTANCE 500 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

    boolean ledState = false;
    boolean led12state = false;
    boolean led11state = false;
    boolean led10state = false;
    boolean led9state = false;
    boolean led8state = false;
    int pingSensorIn;

volatile byte state = LOW;

int leftspeedmax = 255;
int rightspeedmax = 255;
int leftspeed;
int rightspeed;
int enableL = 44;
int enableR = 46;
int Left1 = 36;
int Left2 = 38;
int Right1 = 40;
int Right2 = 42;
int isrunning;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

    void setup(){
      pinMode(ledPin,OUTPUT);
      pinMode(physicalPin, OUTPUT);
      pinMode(11, OUTPUT);
      pinMode(10, OUTPUT);
      pinMode(9, OUTPUT);
      pinMode(8, OUTPUT);
      
      digitalWrite(ledPin,ledState);
      digitalWrite(physicalPin, led12state);
      digitalWrite(11, led11state);
      digitalWrite(10, led10state);
      digitalWrite(9, led9state);
      digitalWrite(8, led8state);
      
	Serial.begin(9600);
    }
     
    void loop() {
    	forward(150,leftspeedmax, rightspeedmax);
    	while (isrunning) {
    check();
    	}
   
    }
     
    void check(){
    
    LedBlinkAction.check();
    physicalPixelAction.check(); //trigger every 50 millisecond
    blink11action.check();
    blink10action.check();
    blink9action.check();
    blink8action.check();
    pingAction.check();
    StopMotorAction.check();
    
    }
     
    
    void pingdo() {
    unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
 /* Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_IN); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println(" in");
  */
  pingSensorIn = (uS / US_ROUNDTRIP_IN);
      
      if (pingSensorIn >= 50)  
      {
      	leftspeed = leftspeedmax - 50;
      	rightspeed = rightspeedmax - 50;
      	Speed(leftspeed, rightspeed);
      }
       if (pingSensorIn > 10 && pingSensorIn <= 30) 
      {
      	leftspeed = leftspeedmax - 75;
      	rightspeed = leftspeedmax - 75;
      	Speed(leftspeed, rightspeed);
      }
      
		if (pingSensorIn <= 9)
		{
			stop();
			leftspeed = leftspeedmax;
			rightspeed =rightspeedmax;
			leftTight(700, leftspeed, rightspeed);
			while (isrunning) {
				check();
			}
			stop();
		}
    } 
     
    void blink() 
    {
    	ledState ? ledState=false : ledState=true;
    	digitalWrite(13, ledState);
    }
   
    void physicalPixel()
    {
    	led12state ? led12state=false : led12state=true;
      digitalWrite(12,led12state);
    
    }
     
     void blink11() {
     		led11state ? led11state=false : led11state=true;
      digitalWrite(11,led11state);
     }
     
     void blink10() {
     		led10state ? led10state=false : led10state=true;
      digitalWrite(10,led10state);
     }
     
     void blink9() {
     		led9state ? led9state=false : led9state=true;
      digitalWrite(9,led9state);
     }
     
     void blink8() {
     		led8state ? led8state=false : led8state=true;
      digitalWrite(8,led8state);
     }
     
   void Speed(int speedL, int speedR) {
	//digitalWrite(_Left1, LOW);
	//digitalWrite(_Left2, LOW);
	//digitalWrite(_Right1, LOW);
	//digitalWrite(_Right2, LOW);
	analogWrite(enableL, speedL);
	analogWrite(enableR, speedR);

}

void stop() {

	digitalWrite(Left1, LOW);
	digitalWrite(Left2, LOW);
	digitalWrite(Right1, LOW);
	digitalWrite(Right2, LOW);
	Speed(0,0);
	isrunning = 0;
}

void ramp() {
	Speed(255,255);
}

void forward(int OnTime, int speedL, int speedR) {
	ramp();
	digitalWrite(Left1, LOW);
	digitalWrite(Left2, HIGH);
	digitalWrite(Right1, LOW);
	digitalWrite(Right2, HIGH);
	StopMotorAction.disable();
	TimedAction StopMotorAction = TimedAction(OnTime, stop);
	StopMotorAction.enable();
	isrunning = 1;
	Speed(speedL, speedR);

}

void back(int OnTime, int speedL, int speedR) {
	ramp();
	digitalWrite(Left1, HIGH);
	digitalWrite(Left2, LOW);
	digitalWrite(Right1, HIGH);
	digitalWrite(Right2, LOW);
	StopMotorAction.disable();
	TimedAction StopMotorAction = TimedAction(OnTime, stop);
	StopMotorAction.enable();
	isrunning = 1;
	Speed(speedL, speedR);
}

void rightTight(int OnTime,int speedL, int speedR) {
	ramp();
	digitalWrite(Left1, LOW);
	digitalWrite(Left2, HIGH);
	digitalWrite(Right1, HIGH);
	digitalWrite(Right2, LOW);
	StopMotorAction.disable();
	TimedAction StopMotorAction = TimedAction(OnTime, stop);
	StopMotorAction.enable();
	isrunning = 1;
	Speed(speedL, speedR);
}

void leftTight(int OnTime,int speedL, int speedR) {
	ramp();
	digitalWrite(Left1, HIGH);
	digitalWrite(Left2, LOW);
	digitalWrite(Right1, LOW);
	digitalWrite(Right2, HIGH);
	StopMotorAction.disable();
	TimedAction StopMotorAction = TimedAction(OnTime, stop);
	StopMotorAction.enable();
	isrunning = 1;
	Speed(speedL, speedR);
}

void right(int OnTime, int speedL, int speedR) {
	Speed(255,0);
	digitalWrite(Left1, LOW);
	digitalWrite(Left2, HIGH);
	digitalWrite(Right1, LOW);
	digitalWrite(Right2, LOW);
	analogWrite(enableR, 0);
	StopMotorAction.disable();
	TimedAction StopMotorAction = TimedAction(OnTime, stop);
	StopMotorAction.enable();
	isrunning = 1;
	Speed(speedL, 0);
}

void left(int OnTime,int speedL, int speedR) {
	Speed(0,255);
	digitalWrite(Left1, LOW);
	digitalWrite(Left2, LOW);
	digitalWrite(Right1, LOW);
	digitalWrite(Right2, HIGH);
	analogWrite(enableL, 0);
	StopMotorAction.disable();
	TimedAction StopMotorAction = TimedAction(OnTime, stop);
	StopMotorAction.enable();
	isrunning = 1;
	Speed(0,speedR);
}

