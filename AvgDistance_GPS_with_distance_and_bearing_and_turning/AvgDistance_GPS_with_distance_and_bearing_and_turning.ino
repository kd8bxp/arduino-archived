/* Notes to myself - distanceBetween is returned in Meters - probably should take an avg reading of the gps to get good distances as it seems to drift a bit */
// steering d3
// drive d5 
//GPS 8, 9
//ping sensor d11 (not ect included)

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h> //I2C Arduino Library
#include <Servo.h>

Servo steering;
Servo esc;

static const int RXPin = 9, TXPin = 8;
static const int GPSBaud = 9600;

static const double curve_lat = 39.43766483, curve_lon = -84.36852637;

const int hmc5883Address = 0x1E; //0011110b, I2C 7bit address of HMC5883
const byte hmc5883ModeRegister      = 0x02;
const byte hmcContinuousMode        = 0x00;
const byte hmcDataOutputXMSBAddress = 0x03;
#define DEC_ANGLE -1.047

int compassheading;
int gpsbearing;
int gpsdistance;
int turn;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

float distanceAVG;

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
	steering.attach(3);
	esc.attach(5);
  Wire.begin();
 
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(hmc5883Address); //open communication with HMC5883
  Wire.write(hmc5883ModeRegister); //select mode register
  Wire.write(hmcContinuousMode);   //continuous measurement mode
  Wire.endTransmission();
  
  //wait for vaild GPS Inforamtion
vaildgps();
	
}

void loop()
{

 smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));


//get compass heading
compassheading = readcompass();

//get GPS bearing
gpsbearing = GPSBearing();

//get GPS Distance returned in FT
gpsdistance = GPSDistance();

//calculate Turn
calcTurn();

Serial.print ("Compass heading: ");
Serial.println(compassheading);
Serial.print("GPS Bearing: ");
Serial.println(gpsbearing);
Serial.print("GPS Distance in FT: ");
Serial.println(gpsdistance);
Serial.println();

if (gpsdistance <= 1) {
	Serial.println("Arrived at Destination!");
	done();
}

Serial.print("Turn: ");
if (turn == 0) {
	Serial.println("Straight");
	straight();
	}	
if (turn == -1) {
	Serial.println("Left");
	left();
	}
if (turn == 1) {
	Serial.println("Right");
	right();
}

} 

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}

float readcompass() {
	 int x,y,z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(hmc5883Address);
  Wire.write(hmcDataOutputXMSBAddress); //select register 3, X MSB register
  Wire.endTransmission();
 
 
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(hmc5883Address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
 
  //Print out values of each axis
 // Serial.print("x: ");
 // Serial.print(x);
 // Serial.print("  y: ");
 // Serial.print(y);
 // Serial.print("  z: ");
 // Serial.print(z);
 
 float heading = atan2(y, x);
 heading += DEC_ANGLE;
 
 if (heading < 0) heading += 2*PI;
 if (heading > 2*PI) heading -= 2*PI;
 
 float headingDegrees = heading * 180/M_PI;
 
 // int angle = atan2(-y , x) / M_PI * 180; // angle is atan(-y/x)
 // if(angle < 0)
 //    angle = angle  + 360; // angle from 0 to 359 instead of plus/minus 180
//  Serial.print(" Direction = ");
//  Serial.println(headingDegrees);
 
  delay(250);
return ((int)headingDegrees);

}

float GPSBearing() {
	 double courseTocurve =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      curve_lat, 
      curve_lon);

//Serial.print("Course: ");
 // printFloat(courseTocurve, gps.location.isValid(), 7, 1);

  const char *cardinalTocurve = TinyGPSPlus::cardinal(courseTocurve);

//Serial.print(" Heading: ");
//  printStr(gps.location.isValid() ? cardinalTocurve : "*** ", 6);
//Serial.println();
return ((int)cardinalTocurve);
}

float GPSDistance() {
distanceAVG = 0;
	for (int count=1; count<=3000; count++) {
  float distanceMToCurve =
    (float)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      curve_lat, 
      curve_lon);

distanceAVG = distanceAVG + distanceMToCurve;
}

distanceAVG = (distanceAVG / 3000) * 3.28084;

//Serial.print("AVG Distance to Curve: ");
//  printFloat(distanceAVG, gps.location.isValid(), 9, 6);
//Serial.println();

return ((int)distanceAVG);

}

void calcTurn() {
	int newheading = gpsbearing - compassheading;
	if (newheading < -180) newheading +=360;
	if (newheading > 180) newheading -=360;
	
	if (abs(newheading) <= 5) turn = 0;
	else if (newheading < 0) turn = -1;
	else if (newheading > 0) turn = 1;
	else turn = 0;
}

void vaildgps() {
	Serial.println("Waiting for GPS vaild...");
//	Serial.println(gps.location.isValid());
	smartDelay(1000);
	if (gps.location.isValid() <= 0) {vaildgps();}
//Serial.println(gps.passedChecksum());

	
}

void straight() {
	steering.write(100); //staight-ish
	move();
}

void left() {
	steering.write(150); //full left
	move();
}

void right() {
	steering.write(50); //full right
	move();
}

void move() {
	checkping();
	esc.writeMicroseconds(745); //slow
	delay(90);
	esc.write(0);
	delay(1000);
}

void checkping() {
	
}

void done() {
	done();
}