//Jun 22 - 2015 - Changed Wire.send to Wire.write and Wire.receive to Wire.read
//also changed "NewSoftSerial.h" to the stand "SoftwareSerial.h" not sure if there is a difference in how the two work.
// steering d3
// drive d4 
//GPS 8, 9
//ping sensor d11 (not ect included)

#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>  
#include <Wire.h>
#define speedesc 125
int reverse=0;
int HMC6352Address = 0x1E; // 0x42;
int slaveAddress;
byte headingData[2];
int i, headingValue;
     // tiny gps library code
  float flat, flon;
   float heading=0;
int headinggps;
int buzzing;
int buttonreaddone;
Servo servo;
Servo esc;
TinyGPS gps;
SoftwareSerial nss(9, 8);    // rx and tx pins in use
#define buzzer 7
int radius;
int x4=0;
#define buttons 0
int waypoints=1;   // number of waypoints
int waycont=1;
float x2lat;
float x2lon;
float flat2=39.43766483;   //waypoint 1    //28.324603,-82.267419 home testing
float flon2=-84.36852637;
float flat3;       //waypoint 2
float flon3;
float flat4;     //waypoint 3
float flon4;
float flat5;    // waypoint   4
float flon5;
float flat6;
float flon6;
float flat7;
float flon7;
float flat8;
float flon8;
float flat9;
float flon9;
float flat10;
float flon10;
float flat11;
float flon11;
float flat12;
float flon12;
float flat13;
float flon13;
void gpsdump(TinyGPS &gps);
bool feedgps();
void printFloat(double f, int digits = 2);
int serv;
void setup()
{
  slaveAddress = HMC6352Address >> 1;  
Wire.begin();
  Serial.begin(115200);
  nss.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(buttons, INPUT);
servo.attach(3); //9
esc.attach(4); //10
esc.write(80);
delay(5000);
buzz();
}

void loop()
{ //-------------------------------------------- this is all gps stuff. This is just the example code given from tiny gps library.
//----------------------------------------------I deleted the part of the code for the time and date. I did not want it.

  bool newdata = false;
  unsigned long start = millis();
  while (millis() - start < 250)            // get new data every 1/4 of a second
  {
    if (feedgps())
      newdata = true;
  }
  
  if (newdata)
  {
    Serial.println("Acquired Data");
    Serial.println("-------------");
    gpsdump(gps);
    Serial.println("-------------");
   Serial.println();
  }
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0)
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
 Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print("."); 
  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint; 
  } 
}

void gpsdump(TinyGPS &gps)
{
  long lat, lon;

  unsigned long age, date, time, chars;

  unsigned short sentences, failed;

  
  feedgps(); // If we don't feed the gps during this long routine, we may drop characters and get checksum errors

  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat/Long(float): "); printFloat(flat, 7); Serial.print(", "); printFloat(flon, 7);   // print our current posistion
  Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");
feedgps();

  gps.stats(&chars, &sentences, &failed);
  Serial.print("Stats: characters: "); Serial.print(chars); Serial.print(" sentences: "); Serial.print(sentences); Serial.print(" failed checksum: "); Serial.println(failed);
  if(buttonreaddone!=5){
    buttonread();
  }
  if(buttonreaddone==5){
distance();
  }
}
  
bool feedgps()
{
  while (nss.available())
  {
    if (gps.encode(nss.read()))
      return true;
  }
  return false;
}
//------------------------------below is the part of the code where everything is calculated
void distance(){
  if(waycont==1){
 x2lat = flat2;      // setting x2lat and x2lon equal to our first waypoint
 x2lon = flon2;   
  }
  if(waycont==2){
    x2lat = flat3;
    x2lon = flon3;
  }
  if(waycont==3){
    x2lat = flat4;
    x2lon = flon4;
  }
  if(waycont==4){
  x2lat = flat5;
  x2lon = flon5;
  }
   if(waycont==5){
  x2lat = flat6;
  x2lon = flon6;
  }
    if(waycont==6){
  x2lat = flat7;
  x2lon = flon7;
  }
     if(waycont==7){
  x2lat = flat8;
  x2lon = flon8;
  }
     if(waycont==8){
  x2lat = flat9;
  x2lon = flon9;
  }
     if(waycont==9){
  x2lat = flat10;
  x2lon = flon10;
  }
     if(waycont==10){
  x2lat = flat11;
  x2lon = flon11;
  }
     if(waycont==11){
  x2lat = flat12;
  x2lon = flon12;
  }
     if(waycont==12){
  x2lat = flat13;
  x2lon = flon13;
  }
 

 float flat1=flat;            
 float flon1=flon;
float dist_calc=0;
float dist_calc2=0;
float diflat=0;
float diflon=0;

//------------------------------------------ distance formula below. Calculates distance from current location to waypoint
diflat=radians(x2lat-flat1);  //notice it must be done in radians
flat1=radians(flat1);
x2lat=radians(x2lat);
diflon=radians((x2lon)-(flon1));

dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
dist_calc2= cos(flat1);
dist_calc2*=cos(x2lat);
dist_calc2*=sin(diflon/2.0);                                       
dist_calc2*=sin(diflon/2.0);
dist_calc +=dist_calc2;

dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));

dist_calc*=6371000.0; //Converting to meters
Serial.println("distance");
Serial.println(dist_calc);
if(dist_calc<radius){
while(waycont==waypoints){
  buzz();
esc.write(80);
delay(3000);
if(reverse==0){
esc.write(60);
delay(1000);
reverse=1;
}
}
waycont+=1;
}
//-----------------------------------------heading formula below. Calculates heading to the waypoint from the current locaiton
 flon1 = radians(flon1);  //also must be done in radians
 x2lon = radians(x2lon);

heading = atan2(sin(x2lon-flon1)*cos(x2lat),cos(flat1)*sin(x2lat)-sin(flat1)*cos(x2lat)*cos(x2lon-flon1)),2*3.1415926535;
heading = heading*180/3.1415926535;  // convert from radians to degrees
int head =heading; 
if(head<0){
  heading+=360;   //if the heading is negative then add 360 to make it positive
}

Serial.println("heading:");
Serial.println(heading);
//-------------------------------------------------------------

int turn=0;
 Wire.beginTransmission(slaveAddress);        //the wire stuff is for the compass module
  Wire.write("A");              
  Wire.endTransmission();
  delay(10);                  
  Wire.requestFrom(slaveAddress, 2);       
  i = 0;
  while(Wire.available() && i < 2)
  { 
    headingData[i] = Wire.read();
    i++;
  }
  headingValue = headingData[0]*256 + headingData[1];
 int pracheading = headingValue / 10;      // this is the heading of the compass
 if(pracheading>0){
   headinggps=pracheading;
 }
//Serial.println("current heaDING:");
//Serial.println(headinggps);
x4=headinggps-heading;   //getting the difference of our current heading to our needed heading

//Serial.println(absolute);

int x5;

//-------------------------------------- below tells us which way we need to turn
if(x4>=-180){
  if(x4<=0){
    turn=8;    //set turn =8 which means "right"         
  }
}
if(x4<-180){
  turn=5;      //set turn = 5 which means "left"
}
if(x4>=0){
  if(x4<180){
    turn=5;   //set turn = 5 which means "left"
  }
}
if(x4>=180){     //set turn =8 which means "right"
  turn=8;
}
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------real important turning stuff. DO NOT TOUCH!!!
float hd = headinggps;
if(hd==heading){
    turn=3;   //then set turn = 3 meaning go "straight"
 
}

if(turn==3){
 
  servo.write(90);  //drive straight
  esc.write(speedesc);
  delay(60);
}
//-------------------------------------------------------------------------------------turn right
if(turn==8){
rightturn();

}
//------------------------------------------------------------------------------------------turn left

if(turn==5){
leftturn();
}

 //-------------------------------------------------------------------------
}

  

void done(){
esc.write(70);  
      done();
}
//----------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//------------------------------------------------------right turning
void rightturn(){
if(headinggps+2>heading){
  if(headinggps-2<heading){
    servo.write(90);
    delay(60);
    return;
  }
}
 x4=headinggps-heading;  
if(x4<-180){
return;
}
if(x4>=0){
  if(x4<180){
  return;
  }
}
servo.write(80);
esc.write(speedesc);
  Wire.beginTransmission(slaveAddress);        //the wire stuff is for the compass module
  Wire.write("A");              
  Wire.endTransmission();
  delay(10);                  
  Wire.requestFrom(slaveAddress, 2);       
  i = 0;
  while(Wire.available() && i < 2)
  { 
    headingData[i] = Wire.read();
    i++;
  }
  headingValue = headingData[0]*256 + headingData[1];  
headinggps = headingValue / 10;      // this is the heading of the compass
rightturn();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------
//----------------------------------------------left turning
void leftturn(){
  if(headinggps+4>heading){
  if(headinggps-4<heading){
  servo.write(90);
    delay(60);
    return;
  }
}
x4=headinggps-heading;  
if(x4>=-180){
  if(x4<=0){
     return;         
  }
}

if(x4>=180){     
  return;
}
servo.write(110);  // turn wheels left
esc.write(speedesc);  //our speed

  Wire.beginTransmission(slaveAddress);        //the wire stuff is for the compass module
  Wire.write("A");              
  Wire.endTransmission();
  delay(10);                  
  Wire.requestFrom(slaveAddress, 2);       
  i = 0;
  while(Wire.available() && i < 2)
  { 
    headingData[i] = Wire.read();
    i++;
  }
headingValue = headingData[0]*256 + headingData[1];  
headinggps = headingValue / 10;      // this is the heading of the compass
leftturn();
}


void buttonread(){
  int butval= analogRead(buttons);
  if(waypoints>0){
  if(butval<100){
    buttonreaddone=5;
        buzz();
      return;
  }
  }
  if(waypoints==0){
    if(butval<100){
      radius+=1;
    buzz();
      return;
    }
  }
      if(butval>600){
     if(waypoints==0){
      flat2=flat;
      flon2=flon;
      waypoints+=1;
         buzz();
      return;
     }
         if(waypoints==1){
      flat3=flat;
      flon3=flon;
      waypoints+=1;
       buzz();
      return;
     }
         if(waypoints==2){
      flat4=flat;
      flon4=flon;
      waypoints+=1;
        buzz();
      return;
     }
         if(waypoints==3){
      flat5=flat;
      flon5=flon;
      waypoints+=1;
        buzz();
      return;
     }
         if(waypoints==4){
      flat6=flat;
      flon6=flon;
      waypoints+=1;
     buzz();
      return;
     }
         if(waypoints==5){
      flat7=flat;
      flon7=flon;
      waypoints+=1;
       buzz();
      return;
     }
          if(waypoints==6){
      flat8=flat;
      flon8=flon;
      waypoints+=1;
         buzz();
      return;
     }
          if(waypoints==7){
      flat9=flat;
      flon9=flon;
      waypoints+=1;
        buzz();
      return;
     }
          if(waypoints==8){
      flat10=flat;
      flon10=flon;
      waypoints+=1;
       buzz();
      return;
     }
          if(waypoints==9){
      flat11=flat;
      flon11=flon;
      waypoints+=1;
     buzz();
      return;
     }
          if(waypoints==10){
      flat12=flat;
      flon12=flon;
      waypoints+=1;
        buzz();
      return;
     }
          if(waypoints==11){
      flat13=flat;
      flon13=flon;
      waypoints+=1;
    buzz();
      return;
     }
        
     }

}
void buzz(){
    while(buzzing<500){
        digitalWrite(buzzer, HIGH);
        delayMicroseconds(250);
        digitalWrite(buzzer, LOW);
        delayMicroseconds(250);
        buzzing++;
      }
      delay(100);
      buzzing=0;
}