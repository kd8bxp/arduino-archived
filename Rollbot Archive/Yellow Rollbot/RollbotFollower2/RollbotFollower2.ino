
/* -----------------------------------------------------------------------------
  Author             : Allen
  Check              : Amy
  Version            : V1.0
  Date               : 17/06/2016
  Description        : Rollbot follows lines
  Company website    : http://www.sunfounder.com
   ---------------------------------------------------------------------------*/

/* Include ------------------------------------------------------------------*/
#include <Wire.h>
#include <MsTimer2.h>
#include "Rollbot.h"
#include "OLEDData.h"

/* PD Adjustment -----------------------------------------------*/
//#define PD_ADJUST  //uncomment this only to adjust the values of PD

/* Print sensors' value by serial monitor -----------------------------------------------*/
//#define Print_Data  //
/* Define------------------------------------------------------------------*/
RollbotMotors Motors;
RollbotReadSensor Sensor;
RollbotOLED OLED;
RollbotLED  LED;
RollbotButton Buttons(10);

#define   LEFT_LED      3
#define   RIGHT_LED     2

int Speed_Dir = 1;

int LEFT_BASE_SPEED = 80;
int RIGHT_BASE_SPEED = 80;
int LEFT_MAX_SPEED = 180;
int RIGHT_MAX_SPEED = 180;
int LEFT_MIN_SPEED = 0;
int RIGHT_MINE_SPEED = 0;
int Speed_L, Speed_R;
float P = 35, D = 450;
int MotorSpeed = 0;
int LastError = 0;
int SignalValue = 0;
int PowerFlag = 0;

void linefollowing();

/*
  - setup function
   ---------------------------------------------------------------------------*/
void setup() {
  //start serial for debug
  Serial.begin(115200);
  Serial.println("Robot starts initialization");

  //start serial for IIC
  Wire.begin();

  //Define LED pins as output
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);

  //initialize OLED  and LED
  OLED.OLED_Init();
  OLED.Clear_Screen(0, 0, 128, 8, 0x00);
  OLED.PowerDisplay(1000);
  OLED.SignalDisplay();
  OLED.OLED_Display(Rollbot,Sunfounder, StartDebug);
  LED.DataOut(0xff);

  //initialize the timer
  MsTimer2::set(2, linefollowing);
}

/*
  - loop function
   ---------------------------------------------------------------------------*/
void loop()
{
#ifdef PD_ADJUST
  PDadjust();
#endif
#ifdef Print_Data
  while (1)
  {
    Sensor.Data_Print();
  }
#endif

  //Display power supply
  PowerFlag = analogRead(A6);
  OLED.PowerDisplay(PowerFlag);

  //Press the button to start the OLED and line following, hold it for a second.
  static int ButtonTime = 1;
  if (ButtonTime == 1)
  {
    Buttons.Button(3, SmilingFace, LongFace, NormalFace, StartDebug, Direction, Foward, Left, Right, Back, TimeNumber, SignalNumber);
  }
  //Automatically detect the reference value of sensors, and then start to follow lines
  static int Times = 1;
  if (Buttons.Time > 5)
  {
    ButtonTime = 0;
    if (Times == 1)
    {
      Times--;
      Sensor.ThresholdCheck(Speed_Dir,150);
      Serial.print("threshold:");
      Serial.println(Sensor.threshold);
      MsTimer2::start();
    }
    OLED.OLED_SignalDisplay(Sensor.OLED_Flag[0], Sensor.OLED_Flag[1], Sensor.OLED_Flag[2], Sensor.OLED_Flag[3], Sensor.OLED_Flag[4]);
  }
}

/*
  - line following function
   ---------------------------------------------------------------------------*/
void linefollowing()
{
  //Restart the timer
  MsTimer2::set(2, linefollowing);
  MsTimer2::stop();

  //Detect the relative position of the Rollman-mini and black line in real time
  SignalValue = Sensor.Read_BlackFlag();

  //PD adjustment
  if ((SignalValue == 0) || (SignalValue == 5) || (SignalValue == 6))
  {
    Motors.Motordrive(Speed_Dir,LEFT_BASE_SPEED, RIGHT_BASE_SPEED);
    digitalWrite(LEFT_LED, LOW); digitalWrite(RIGHT_LED, LOW);
  }
  else
  {
    Motors.Motordrive(Speed_Dir,LEFT_BASE_SPEED, RIGHT_BASE_SPEED);
    if (SignalValue > 0)
    {
      digitalWrite(LEFT_LED, LOW);
      digitalWrite(RIGHT_LED, HIGH);
    }
    else
    {
      digitalWrite(RIGHT_LED, LOW);
      digitalWrite(LEFT_LED, HIGH);
    }
    MotorSpeed = int(P * SignalValue + D * (SignalValue - LastError));
    LastError = 0;
    LastError = SignalValue;
    Speed_L = LEFT_BASE_SPEED + MotorSpeed;
    Speed_R = RIGHT_BASE_SPEED - MotorSpeed;
    if (Speed_R > RIGHT_MAX_SPEED ) Speed_R = RIGHT_MAX_SPEED;
    if (Speed_L > LEFT_MAX_SPEED )  Speed_L = LEFT_MAX_SPEED;
    if (Speed_R < RIGHT_MINE_SPEED) Speed_R = RIGHT_MINE_SPEED;
    if (Speed_L < LEFT_MIN_SPEED)   Speed_L = LEFT_MIN_SPEED;
    Motors.Motordrive(Speed_Dir,Speed_L, Speed_R);
    delay(1);
    Motors.Motordrive(Speed_Dir,LEFT_BASE_SPEED - 15, RIGHT_BASE_SPEED - 15);
  }

  //Reset the timer
  
}

/* Adjust PD through the application
  ---------------------------------------------------------------------------*/
void PDadjust()
{
  char temp = 0;
  while (Serial.available())
  {
    temp = Serial.read();
  }
  if (temp == 'c')
  {
    P = P + 0.5;
    Serial.print(P);
  }
  else if (temp == 'd')
  {
    P = P - 0.5;
    Serial.print(P);
  }
  else if (temp == 'a')
  {
    D--;
    Serial.print(D);
  }
  else if (temp == 'b')
  {
    D++;
    Serial.print(D);
  }
}
