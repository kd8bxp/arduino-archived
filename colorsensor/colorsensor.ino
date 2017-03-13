int redPin = 11;
int greenPin = 10;
int bluePin = 6;

int S2 = 7;
int S3 = 8;
int outPin = 4;

unsigned int pulseWidth;

int rColorStrength;
int gColorStrength;
int bColorStrength;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
pinMode(bluePin, OUTPUT);
digitalWrite(redPin, HIGH);
digitalWrite(greenPin, HIGH);
digitalWrite(bluePin, HIGH);
pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(outPin, INPUT);

}

void loop() {

//RED Component S2 and S3 set LOW

digitalWrite(S2, LOW);
digitalWrite(S3, LOW);

pulseWidth = pulseIn(outPin, LOW);

rColorStrength = pulseWidth/400 - 1;

rColorStrength = (255 - rColorStrength);

//GREEN Component S2 and S3 set HIGH

digitalWrite(S2, HIGH);
digitalWrite(S3, HIGH);

pulseWidth = pulseIn(outPin, LOW);

gColorStrength = pulseWidth/400 - 1;

gColorStrength = (255 - gColorStrength);

//BLUE Component S2 set LOW and S3 set HIGH

digitalWrite(S2, HIGH);
digitalWrite(S3, LOW);

pulseWidth = pulseIn(outPin, LOW);

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

if (rColorStrength>gColorStrength && gColorStrength > bColorStrength) {
  rColorStrength = 0;
  gColorStrength = gColorStrength/2;
  bColorStrength = 255;
}

if (rColorStrength>bColorStrength && bColorStrength > gColorStrength) {
  rColorStrength = 0;
  bColorStrength = bColorStrength/2;
  gColorStrength = 255;
}

if (gColorStrength>rColorStrength && rColorStrength > bColorStrength) {
  gColorStrength = 0;
  rColorStrength = rColorStrength/2;
  bColorStrength = 255;
}

if (gColorStrength>bColorStrength && bColorStrength > rColorStrength) {
  gColorStrength = 0;
  bColorStrength = bColorStrength/2;
  rColorStrength = 255;
}

if (bColorStrength>rColorStrength && rColorStrength > gColorStrength) {
  bColorStrength = 0;
  rColorStrength = rColorStrength/2;
  gColorStrength = 255;
}

if (bColorStrength>gColorStrength && gColorStrength > rColorStrength) {
  bColorStrength = 0;
  gColorStrength = gColorStrength/2;
  rColorStrength = 255;
}

//tweak colors
bColorStrength = bColorStrength * .5;
gColorStrength = gColorStrength * .75;

analogWrite(redPin, rColorStrength);
analogWrite(greenPin, gColorStrength);
analogWrite(bluePin, bColorStrength);

Serial.print(rColorStrength);
Serial.print(",");
Serial.print(gColorStrength);
Serial.print(",");
Serial.println(bColorStrength);
Serial.println("");

delay(250);


}
