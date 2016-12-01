    /*
    ||
    || @file ThreeExamplesAtOnce.pde
    || @version 1.0
    || @author Alexander Brevig
    || @contact alexanderbrevig@gmail.com
    ||
    || @description
    || | This sketch blinks an LED as Blink
    || |             sets a led on or off according to serial buffer as PhysicalPixel
    || |             prints the ascii table as ASCIITable
    || #
    ||
    */
     
    #include <TimedAction.h>
    #include <NewPing.h>
    #include <SPI.h>
#include <string.h>
#include "DHT.h"
 
    //this initializes a TimedAction object that will change the state of an LED every second.
    TimedAction blinkAction                 =       TimedAction(150,blink);
    //this initializes a TimedAction object that will change the state of an LED
    //according to the serial buffer contents, every 50 milliseconds
    TimedAction physicalPixelAction =       TimedAction(1000,physicalPixel);
    //this initializes a TimedAction object that will write tha ascii table to the serial every ten seconds
    TimedAction asciiTableAction    =       TimedAction(10000,asciiTable);
    TimedAction blink11action = TimedAction(500, blink11);
    TimedAction blink10action = TimedAction(250, blink10);
    TimedAction blink9action = TimedAction(100, blink9);
    TimedAction blink8action = TimedAction(750, blink8);
    TimedAction pingAction = TimedAction(15, pingdo);
    TimedAction readTempAction = TimedAction(50, readTemp);
    
    //pin / state variables
    #define ledPin 13
    #define physicalPin 12
    #define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

    boolean ledState = false;
    boolean led12state = false;
    boolean led11state = false;
    boolean led10state = false;
    boolean led9state = false;
    boolean led8state = false;
    int pingSensorIn;
int triggerDistance = 5; //Your Too Close distance in Inches
int triggerDistance2 = 10; //Getting closer distance (Caution!) in Inches
int triggerDistance3 = 30; //No used 
int redLED = 4;
int greenLED = 3;
int blueLED = 2;

const byte laserPin = 17;
const byte interruptPin = 18;
volatile byte state = LOW;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11
 
int tempRedLed = 14;
int tempBlueLed = 16;
int tempGreenLed = 15;

// DHT instance
DHT dht(DHTPIN, DHTTYPE);
                         
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
      
      pinMode(redLED, OUTPUT);
pinMode(greenLED, OUTPUT);
pinMode(blueLED, OUTPUT);

digitalWrite (redLED, HIGH);
digitalWrite (greenLED, HIGH);
digitalWrite (blueLED, HIGH);
 
  pinMode(laserPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  //attachInterrupt(interruptPin, firelaser, LOW);
  attachInterrupt(5, firelaser, LOW);
      pinMode(tempRedLed, OUTPUT);
 pinMode(tempBlueLed, OUTPUT);
 pinMode(tempGreenLed, OUTPUT);
 digitalWrite(tempRedLed, HIGH);
 digitalWrite(tempBlueLed, HIGH);
 digitalWrite(tempGreenLed, HIGH);
  // Initialize DHT sensor
  dht.begin();
      Serial.begin(9600);
    }
     
    void loop() {
    	check();
    }
     
    void check(){
      blinkAction.check(); //trigger every second
      physicalPixelAction.check(); //trigger every 50 millisecond
      asciiTableAction.check(); //trigger every 10 second
    blink11action.check();
    blink10action.check();
    blink9action.check();
    blink8action.check();
    pingAction.check();
    readTempAction.check();
    }
     
     void readTemp() {
     	// Measure the humidity & temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
 
 if (f >= 70.0 && f < 82.0) {
 	digitalWrite(tempRedLed, HIGH);
 	digitalWrite(tempBlueLed, HIGH);
 	digitalWrite(tempGreenLed, LOW);
 	 }
 
 if (f >=82.0) {
 	digitalWrite(tempRedLed, LOW);
 	digitalWrite(tempBlueLed, HIGH);
 	digitalWrite(tempGreenLed, HIGH);
 }
 
 if (f < 70.0) {
 	digitalWrite(tempRedLed, HIGH);
 	digitalWrite(tempBlueLed, LOW);
 	digitalWrite(tempGreenLed, HIGH);
 }
 
  /*
    // Transform to String
    String tempc = String((int) t);
    String tempF = String((int) f);
    String hum = String((int) h);
    String heat = String((int) hi); 
    
  Serial.print("Humidity: "); 
  Serial.print(hum);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(tempc);
  Serial.print(" *C , ");
  Serial.print(f);
  Serial.print(" *F\t  ");
  Serial.print("Heat index: ");
  Serial.print(heat);
  Serial.println(" *F");
   */
     }
     
     void firelaser() {
	digitalWrite(laserPin, state);
  state = !state;
}
    void pingdo() {
    unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
 /* Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_IN); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println(" in");
  */
  pingSensorIn = (uS / US_ROUNDTRIP_IN);
      
      if (pingSensorIn <= triggerDistance)  
      {
      	//To Close - RED Stop!
        digitalWrite(redLED, LOW);
		digitalWrite(greenLED, HIGH);
		digitalWrite(blueLED, HIGH);
	//	Serial.print("RED: ");
	//	Serial.print(pingSensorIn);
	//	Serial.println(" in.");
      }
       if (pingSensorIn > triggerDistance && pingSensorIn < triggerDistance2) 
      {
      	//Almost Close - YELLOW Caution!
        digitalWrite(greenLED, LOW);
		digitalWrite(redLED, LOW);
		digitalWrite(blueLED, HIGH);
	//	Serial.print("Yelow: ");
	//	Serial.print(pingSensorIn);
	//	Serial.println(" in.");
      }
      
		if (pingSensorIn >= triggerDistance2)
		{
		//Clear - GREEN stuff is pretty far from you!	
		digitalWrite(blueLED, HIGH);
		digitalWrite(redLED, HIGH);
		digitalWrite(greenLED, LOW);
	//	Serial.print("Green: ");
	//	Serial.print(pingSensorIn);
	//	Serial.println(" in.");
		}
    } 
     
    //[url=http://arduino.cc/en/Tutorial/Blink]Examples->Digital->Blink[/url]
    void blink(){
      ledState ? ledState=false : ledState=true;
      digitalWrite(ledPin,ledState);
    }
     
    //[url=http://arduino.cc/en/Tutorial/PhysicalPixel]Examples->Digital->PhysicalPixel[/url]
    void physicalPixel()
    {
    	led12state ? led12state=false : led12state=true;
      digitalWrite(12,led12state);
      
     // if (Serial.available()) {
     //  byte val = Serial.read();
     //   if (val == 'H') {
     //     digitalWrite(physicalPin, HIGH);
     //   }
     //   if (val == 'L') {
     //     digitalWrite(physicalPin, LOW);
     //   }
     // }
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
     
    //[url=http://arduino.cc/en/Tutorial/ASCIITable]Examples->Digital->ASCIITable[/url]
    void asciiTable()
    {
      byte number = 33; // first visible character '!' is #33
      // print until we have printed last visible character '~' #126 ...
      while(number <= 126) {
        //Serial.print(number, BYTE);    // prints value unaltered, first will be '!'
     
        Serial.print(", dec: ");
        Serial.print(number);          // prints value as string in decimal (base 10)
        // Serial.print(number, DEC);  // this also works
     
        Serial.print(", hex: ");
        Serial.print(number, HEX);     // prints value as string in hexadecimal (base 16)
     
        Serial.print(", oct: ");
        Serial.print(number, OCT);     // prints value as string in octal (base 8)
     
        Serial.print(", bin: ");
        Serial.println(number, BIN);   // prints value as string in binary (base 2)
                                                                             // also prints ending line break
        number++; // to the next character
        check();
      }
      
      //asciiTableAction.disable();
    }
     
