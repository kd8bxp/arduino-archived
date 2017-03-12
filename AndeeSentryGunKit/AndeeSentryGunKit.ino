/************************************************************
  [Annikken Andee Sentry Gun With Andee U V1.0]
  ================
  by Robin Ha <robin@annikken.com>

  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  http://resources.annikken.com
  
  Check out our Blog for regular updates and fun!
  http://annikken.com/blog
************************************************************/


#include <SPI.h>
#include <Andee.h>
#include <Servo.h>

AndeeHelper WatchTitle;
AndeeHelper WatchBack;
AndeeHelper WatchButton1;//Up
AndeeHelper WatchButton2;//Down
AndeeHelper WatchButton3;//Left
AndeeHelper WatchButton4;//Right


AndeeHelper button_PC;
AndeeHelper button_SentryControl;
AndeeHelper button_Up;
AndeeHelper button_Down;
AndeeHelper button_Left;
AndeeHelper button_Right;
AndeeHelper button_Reset;
AndeeHelper FireButton;
AndeeHelper Blank;


AndeeHelper TitleBox;//Right
Servo HServo;
Servo VServo;
int Mode = 0;
bool Update = false;
int HPin = A1;
int VPin = A0;
int MissilePin = 4;
int LaserPin = 9;
bool laserState = false;

int HPos = 90;
int VPos = 10;
int Sensitivity = 10;
char userInput[32];

bool Clockwise = true;
int change = 2;
int UpdateScreen;
boolean lastIsConnected;


char newBluetoothName[] = "Andee Sentry Gun"; // New device name
char cmdReply[32]; // String buffer
char commandString[32]; // String to store the new device name and device command into one

void setup()
{
  UpdateScreen = true;
  lastIsConnected = false;

  Serial.begin(9600);
  Serial.println("setup1");
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays


  sprintf(commandString, "SET BT NAME %s", newBluetoothName);
  // Send command to change device name
  Andee.sendCommand(commandString, cmdReply);

  setInitialData(); // Define object types and their appearance
  Serial.println("setup2");

  delay(1000);
  Serial.println("startloop");
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone

void updateScreen() {
  if (Update == true) {
    Update = false;
    if (Mode == 0) {
      setMode0();
    }
    else if (Mode == 1) {
      setMode1();
    }
    else if (Mode == 2) {
      setMode2();
    }
    else if (Mode == 4) {
      setMode4();
    }

    delay(100);
    WatchTitle.update();
    delay(100);
    WatchButton1.update();
    delay(100);
    WatchButton2.update();
    delay(100);
    WatchButton3.update();
    delay(100);
    WatchButton4.update();




    delay(100);
    Andee.hideScreen();
    delay(100);
    Andee.showScreen();
  }
}
int xxx=0;
int cur_isConnected;
int CheckConnectionCounter = 0;
void loop()
{
  // Main Page

  long T0 = millis();
  CheckConnectionCounter++;
  if(CheckConnectionCounter==10){
    cur_isConnected = Andee.isConnected();
    CheckConnectionCounter=0;
  }
  if (lastIsConnected == false && cur_isConnected == 1) {
    //from DC to Connected
    delay(200);
    Serial.println("connected");
    Andee.clear();
    setInitialData();
    delay(200);
    Mode = 0;
    Update = true;
    Serial.println("connected2");
    lastIsConnected = true;

  }

  else if (lastIsConnected == true && cur_isConnected == 0) {
    delay(300);

    if (Andee.isConnected() == 0) {
      Serial.println("disconnected");
      lastIsConnected = false;
      Mode = 0;
    Update = true;

    }

  }

  long T1 = millis();
  updateScreen();

  long T2 = millis();

  Andee.checkButtons();

  long T3 = millis();
  if (Mode == 1) {
    Mode1();

    delay(10);
  }
  else if (Mode == 2) {
    Mode2();

    delay(50);
  }
  else if (Mode == 4) {
    Mode4();

    delay(200);
  }
  else if (Mode == 0) {
    Mode0();

    delay(10);
  }
  else {

    delay(200);
  }

//delay(1000);

  long T4 = millis();

}

void setInitialData()
{
  TitleBox.setId(2);
  TitleBox.setType(DATA_OUT);
  TitleBox.setLocation(0, 0, FULL);
  TitleBox.setTitle("Annikken Andee");
  TitleBox.setData("Andee Sentry Gun");
  TitleBox.setColor("FFEE7621");
  TitleBox.setTitleColor("50FFFFFF");
  delay(100);

  Serial.println("  Set Initial Data");

  pinMode(HPin, OUTPUT);
  pinMode(VPin, OUTPUT);
  pinMode(MissilePin, OUTPUT);
  pinMode(LaserPin, OUTPUT);
  digitalWrite(MissilePin, LOW);
  laserState = LOW;
  digitalWrite(LaserPin, laserState);

  HServo.attach(HPin);
  VServo.attach(VPin);

  HPos = 90;
  VPos = 10;
  HServo.write(HPos);
  VServo.write(VPos);
  Serial.println("  Set Initial Data End");


}


