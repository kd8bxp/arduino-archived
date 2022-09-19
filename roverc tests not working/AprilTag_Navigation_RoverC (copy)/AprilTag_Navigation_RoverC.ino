#include "src/M5StickC/M5StickC.h"
#include "src/ArduinoJson5/ArduinoJson.h"
#include "RoverC.h"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

bool setGridFlag = true; //false;
int setGridTarget = 0;

typedef struct {
int x;
int y;
int w;
int h;
int id;
int cx;
int cy;
int rotation;
}camera;

camera tagInfo;

enum
{
    noTarget = 0,
    left,
    right,
    straight,
    stop
};

//setup for moving in a straight line based on code by GEH00073 https://qiita.com/airpocket/items/0cfb9e3e1a202c2f00df Jan 21-22, 2020
const uint16_t threshold = 20; //±kThreshold, the car will go straight defo = 20
double vpara = .6;               //Speed Change
int movetime = 100;              
int waitcam = 200; 
uint8_t state = 0;  // Car's movement status
int rotate = 1;     //回転方向指定
int dx_old = 0;     //ターゲットの過去位置
int16_t dx;             

//How many markers are on the grid, compass headings are realtive to front of RoverC
//int numberTags = 49; 
//int startTag = 0;
//int endTag = 48;
int topTag = 200; //Boarder tag number used to mark up (realitive to front of RoverC)
int bottomTag = 202; //Boarder tag number used to mark down (realitive to front of RoverC)
int rightTag = 201; //Boarder tag number used to mark right side (realitive to front of RoverC)
int leftTag = 203; //Boarder tag number used to mark left side (realitive to front of RoverC)
int gridX = 7; //number of squares in the x coridants
int gridY = 7; //number of squares in the y coridants


bool startMoving = false; //maybe same idea as setGridFlag (?) I have to think about it
int currentTag; //currently read ID from tag
int oldTag = -100; //previously read ID from tag init set to impossable number
int roverCFacing; //0 = top, 1 = right, 2 = bottom, 3= left
int startingTag; //starting tag

void readSerialEvent();
void decodeJson();
void moveToTarget();

void screenOff() {
  Wire1.beginTransmission(0x34);
  Wire1.write(0x12);
  Wire1.write(0b01001011); //LDO2, aka OLED_VDD, off
  Wire1.endTransmission();
}

void screenOn() {
 Wire1.beginTransmission(0x34);
 Wire1.write(0x12);
 Wire1.write(0x4d); // Enable LDO2, aka OLED_VDD
 Wire1.endTransmission();  
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 33, 32);
  delay(100);
  Wire.begin(0,26,10000);
  Wire1.begin(21, 22);
  //screenOff();
  RoverC_Init();
  
}

void loop() {
 
if (setGridFlag == true) { moveToTarget(); }
}
