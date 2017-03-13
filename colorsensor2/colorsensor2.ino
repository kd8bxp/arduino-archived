#include <Adafruit_NeoPixel.h>

int S2 = 3;
int S3 = 4;
int outPin = 2;

unsigned int pulseWidth;

int rColorStrength;
int gColorStrength;
int bColorStrength;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1,8, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
strip.begin();
strip.show();

pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(outPin, INPUT);

}

void loop() {

//RED Component S2 and S3 set LOW

digitalWrite(S2, LOW);
digitalWrite(S3, LOW);

pulseWidth = pulseIn(outPin, HIGH);

rColorStrength = pulseWidth/400-1 ;

rColorStrength = (255 - rColorStrength);

//GREEN Component S2 and S3 set HIGH

digitalWrite(S2, HIGH);
digitalWrite(S3, HIGH);

pulseWidth = pulseIn(outPin, HIGH);

gColorStrength = pulseWidth/400 - 1;

gColorStrength = (255 - gColorStrength);

//BLUE Component S2 set LOW and S3 set HIGH

digitalWrite(S2, LOW);
digitalWrite(S3, HIGH);

pulseWidth = pulseIn(outPin, HIGH);

bColorStrength = pulseWidth/400 - 1;

bColorStrength = (255 - bColorStrength);
/*
if (rColorStrength > bColorStrength && rColorStrength > gColorStrength) {
  digitalWrite(redPin, LOW);
  digitalWrite(bluePin, HIGH);
  digitalWrite(greenPin, HIGH);
}

if (gColorStrength > bColorStrength && gColorStrength > rColorStrength) {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

if (bColorStrength > gColorStrength && bColorStrength > rColorStrength) {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}
*/

if (rColorStrength > gColorStrength && gColorStrength > bColorStrength) {
  rColorStrength = 255;
  gColorStrength = gColorStrength/2;
  bColorStrength = 0;
}

if (rColorStrength > bColorStrength && bColorStrength > gColorStrength) {
  rColorStrength = 255;
  bColorStrength = bColorStrength/2;
  gColorStrength = 0;
}

if (gColorStrength > rColorStrength && rColorStrength > bColorStrength) {
  gColorStrength = 255;
  rColorStrength = rColorStrength/2;
  bColorStrength = 0;
}

if (gColorStrength > bColorStrength && bColorStrength > rColorStrength) {
  gColorStrength = 255;
  bColorStrength = bColorStrength/2;
  rColorStrength = 0;
}

if (bColorStrength > rColorStrength && rColorStrength > gColorStrength) {
  bColorStrength = 255;
  rColorStrength = rColorStrength/2;
  gColorStrength = 0;
}

if (bColorStrength > gColorStrength && gColorStrength > rColorStrength) {
  bColorStrength = 255;
  gColorStrength = gColorStrength/2;
  rColorStrength = 0;
}

//tweak colors
//bColorStrength = bColorStrength * .75;
//gColorStrength = gColorStrength * .75;
//rColorStrength = rColorStrength * .75;

strip.setPixelColor(0, gColorStrength, rColorStrength, bColorStrength);
strip.show();

Serial.print(rColorStrength);
Serial.print(",");
Serial.print(gColorStrength);
Serial.print(",");
Serial.println(bColorStrength);
Serial.println("");

delay(250);


}
