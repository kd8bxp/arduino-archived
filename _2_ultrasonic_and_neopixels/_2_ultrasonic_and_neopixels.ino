#include <NewPing.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

int pingSensorInF;
int pingSensorInR;
int triggerDistance = 5; // Your too close distance in inches
int triggerDistance2 = 10; // getting a little too close (Caution!) in inches
int triggerDistance3 = 30; //not used (yet)
int uS;

#define ForwardTriggerPin 5 // Arduino pin tied to trigger pin on forward ultrasonic sensor
#define ForwardEchoPin 6 // Arduino pin tied to echo pin on the ultrasonic sensor
#define ReverseTriggerPin 7 //Arduino pin for reverse pin sensor
#define ReverseEchoPin 8 //Arduino pin for reverse echo pin sensor
#define max_distance 500 //maximum distance we want to ping for (in centimeters)  Maximum sensor distance is rated at 400 - 500cm.

#define neopixelpin 4 //output pin for neopixels
#define neopixelnumber 8 //how many neopixels are connected

NewPing forwardping (ForwardTriggerPin, ForwardEchoPin, max_distance);
NewPing reverseping (ReverseTriggerPin, ReverseEchoPin, max_distance);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(neopixelnumber, neopixelpin, NEO_GRB + NEO_KHZ800);

void setup() {
	Serial.begin(9600);
	strip.begin();
	strip.show();
	strip.setPixelColor(2, 0,0,255);
	strip.show();
}

void loop() {
	
	uS = forwardping.ping();
	pingSensorInF = (uS / US_ROUNDTRIP_IN);
	//delay(500);
	uS = reverseping.ping();
	pingSensorInR = (uS / US_ROUNDTRIP_IN);
	
	if (pingSensorInF <= triggerDistance) { 
		for (int i=3; i<neopixelnumber; i++)
		{strip.setPixelColor(i, 255, 0, 0);
		strip.show();
		}
	}
	
	if (pingSensorInF > triggerDistance && pingSensorInF < triggerDistance2) {
		for (int i=3; i<neopixelnumber; i++) {
			strip.setPixelColor(i, 255, 255, 0);
			strip.show();
		}
	}
	
	if (pingSensorInF > triggerDistance2) {
		for (int i=3; i<neopixelnumber; i++) {
			strip.setPixelColor(i, 0, 255, 0);
			strip.show();
		}
	}
	
	if (pingSensorInR <= triggerDistance) {
		for (int i = 0; i<2; i++) {
			strip.setPixelColor(i, 255, 0, 0);
			strip.show();
		}
	}
	
	if (pingSensorInR > triggerDistance && pingSensorInR < triggerDistance2) {
		for (int i = 0; i<2; i++) {
			strip.setPixelColor(i, 255, 255, 0);
			strip.show();
		}
	}
	
	if (pingSensorInR > triggerDistance2) {
		for (int  i = 0; i<2; i++){
			strip.setPixelColor(i, 0, 255, 0);
			strip.show();
		}
	}
	
	/*
		for (int i=0; i<neopixelnumber; i++){
		strip.setPixelColor(i, 0, 0, 0);
		strip.show();
					}
	*/
	strip.setPixelColor(2, 0, 0, 255);
	strip.show();
	
}

