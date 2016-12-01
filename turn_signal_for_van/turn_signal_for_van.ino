#include <Adafruit_NeoPixel.h>
 
#define N_LEDS 10
#define PIN 3
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_RGB + NEO_KHZ800);


int leftin = 8;
int rightin = 10;
 
void setup() {
strip.begin();
strip.setBrightness(250);
pinMode(leftin, INPUT);
pinMode(rightin, INPUT);
}
 
int pos = 0, dir = 1; // Position, direction of "eye"
 
void loop() {

strip.show();
if (digitalRead(leftin) == LOW) { left();}
if (digitalRead(rightin) == LOW) {right();}

}

void stop() {
	strip.setPixelColor(0, 255, 0, 0);
	strip.setPixelColor(1, 255, 0, 0);
	strip.setPixelColor(2, 255, 0, 0);
	strip.setPixelColor(3, 255, 0, 0);
	strip.setPixelColor(4, 255, 0, 0);
	strip.setPixelColor(5, 255, 0, 0);
	strip.setPixelColor(6, 255, 0, 0);
	strip.setPixelColor(7, 255, 0, 0);
	strip.setPixelColor(8, 255, 0, 0);
	strip.setPixelColor(9, 255, 0, 0);
	
}

void left() {
	for (int i=0; i<=50; i++) {
		strip.setPixelColor(4, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(4, 0, 0, 0);
		delay(25);
		strip.setPixelColor(3, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(3, 0, 0, 0);
		delay(25);
		strip.setPixelColor(2, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(2, 0, 0, 0);
		delay(25);
		strip.setPixelColor(1, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(1, 0, 0, 0);
		delay(25);
		strip.setPixelColor(0, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(0, 0, 0, 0);
		delay(25);

}

strip.setPixelColor(0, 0, 0, 0);
strip.setPixelColor(1, 0, 0, 0);
strip.setPixelColor(2, 0, 0, 0);
strip.setPixelColor(3, 0, 0, 0);
strip.setPixelColor(4, 0, 0, 0);

}


void right() {
	for (int i=0; i<=50; i++) {
		strip.setPixelColor(5, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(5, 0, 0, 0);
		delay(25);
		strip.setPixelColor(6, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(6, 0, 0, 0);
		delay(25);
		strip.setPixelColor(7, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(7, 0, 0, 0);
		delay(25);
		strip.setPixelColor(8, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(8, 0, 0, 0);
		delay(25);
		strip.setPixelColor(9, 255, 0, 0);
		strip.show();
		delay(60);
		strip.setPixelColor(9, 0, 0, 0);
		delay(25);

}

strip.setPixelColor(5, 0, 0, 0);
strip.setPixelColor(6, 0, 0, 0);
strip.setPixelColor(7, 0, 0, 0);
strip.setPixelColor(8, 0, 0, 0);
strip.setPixelColor(9, 0, 0, 0);

}



















