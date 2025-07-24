/* -----------------------------------------------------------------------------
  Author             : Allen
  Check              : Amy
  Version            : V1.0
  Date               : 17/06/2016
  Description        : Control the car running by application Rollman (download on GooglePlay)
  Company website    : http://www.sunfounder.com
   ---------------------------------------------------------------------------*/

/* Include -------------------------------------------------------------------*/
#include <Wire.h>
#include "Rollbot.h"
#include "OLEDData.h"

/* Define -------------------------------------------------------------------*/
RollbotMotors Motors;
RollbotLED  LED;
RollbotOLED OLED;
RollbotReadSensor Sensor;
RollbotButton Buttons(10);
RollbotBuzzer Music(9);

#define   LEFT_LED      3
#define   RIGHT_LED     2

int Speed_Dir = 1;
int LEFT_BASE_SPEED = 90;
int RIGHT_BASE_SPEED = 90;
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
int DirectionFlag = 0;
int BarrierTime = 0;
int Barrier_Flag = 0;
int DirectionFlagBackup = 0;
int Length = 0;

//int ClearFlag = 1;

/*
 - Scale
* ---------------------------------------------------------------------------*/
int tune[] = 
{
  NOTE_C4,NOTE_C4,NOTE_C3,NOTE_C4,
  NOTE_C3,NOTE_C1,NOTE_C1,
  NOTE_C4,NOTE_C4,NOTE_C3,NOTE_C4,
  NOTE_C3,NOTE_C1,NOTE_C1,
};

/*
 - Frequency
* ---------------------------------------------------------------------------*/
float duration[]=
{
  1.5,0.5,0.5,0.5,
  1,0.5,1,  
  1.5,0.5,0.5,0.5,
  1,0.5,1,
};

void BarrierCalculate();
void linefollowing();
void turn_back();
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
  Length = sizeof(tune) / sizeof(tune[0]);

  //initialize OLED  and LED
  OLED.OLED_Init();
  OLED.Clear_Screen(0, 0, 128, 8, 0x00);
  OLED.SignalDisplay();
  OLED.PowerDisplay(1000);
  OLED.OLED_Display(Rollbot,Sunfounder, StartDebug);
  LED.DataOut(0xff);
  
}

/*
  - loop function
   ---------------------------------------------------------------------------*/
void loop() {
  //Display power supply
  int PowerFlag = 0;
  PowerFlag = analogRead(A6);
  OLED.PowerDisplay(PowerFlag);

  //Press the button to start the OLED,hold it for a second.
  static int ButtonTime = 1;
  if(ButtonTime == 1)
  {
    Buttons.Button(0, SmilingFace, LongFace, NormalFace, StartDebug, Direction, Foward, Left, Right, Back, TimeNumber, SignalNumber);
  }

  //APP control
  static char ReceiveByte = 0;
  static int Times = 1;

  //Button
  if (Buttons.Time > 5)
  {
    ButtonTime=0;
    if (Times == 1)
    {
      Times--;
      Sensor.ThresholdCheck(Speed_Dir, 150);
    }
    while (Serial.available())
    {
      ReceiveByte = Serial.read();
    }
    
  //When dicerolling,the time to go forward    
    if (ReceiveByte == '1')
    {
      ReceiveByte = 0; DirectionFlag = 1; 
      OLED.SelectFace(1, SmilingFace, LongFace, NormalFace); 
      while (DirectionFlag)
      {
        linefollowing();
      }
      for(int i=50;i>0;i--)
      {linefollowing();}
      Motors.Motorstop();
      BarrierCalculate();
      OLED.SelectFace(2, SmilingFace, LongFace, NormalFace);
    }
    
    else if (ReceiveByte == '2')
    {
      ReceiveByte = 0; DirectionFlag = 2; 
      OLED.SelectFace(1, SmilingFace, LongFace, NormalFace); 
      while (DirectionFlag)
      {
        linefollowing();
      }
      for(int i=50;i>0;i--)
      {linefollowing();}
      Motors.Motorstop();
      BarrierCalculate();
      OLED.SelectFace(2, SmilingFace, LongFace, NormalFace);
    }

    
    else if (ReceiveByte == '3')
    {
      ReceiveByte = 0; DirectionFlag = 3; 
      OLED.SelectFace(1, SmilingFace, LongFace, NormalFace);
      while (DirectionFlag)
      {
        linefollowing();
      }
      for(int i=50;i>0;i--)
      {linefollowing();}
      Motors.Motorstop();
      BarrierCalculate();
      OLED.SelectFace(2, SmilingFace, LongFace, NormalFace);
    }

    
    else if (ReceiveByte == '4')
    {
      ReceiveByte = 0; DirectionFlag = 4; 
      OLED.SelectFace(1, SmilingFace, LongFace, NormalFace); 
      while (DirectionFlag)
      {
        linefollowing();
      }
      for(int i=50;i>0;i--)
      {linefollowing();}
      Motors.Motorstop();
      BarrierCalculate();
      OLED.SelectFace(2, SmilingFace, LongFace, NormalFace);
    }

    
    else if (ReceiveByte == '5')
    {
      ReceiveByte = 0; DirectionFlag = 5; 
      OLED.SelectFace(1, SmilingFace, LongFace, NormalFace); 
      while (DirectionFlag)
      {
        linefollowing();
      }
      for(int i=50;i>0;i--)
      {linefollowing();}
      Motors.Motorstop();
      BarrierCalculate();
      OLED.SelectFace(2, SmilingFace, LongFace, NormalFace);
    }

    
    else if (ReceiveByte == '6')
    {
      ReceiveByte = 0; DirectionFlag = 6; 
      OLED.SelectFace(1, SmilingFace, LongFace, NormalFace); //MsTimer2::start();
      while (DirectionFlag)
      {
        linefollowing();
      }
      for(int i=50;i>0;i--)
      {linefollowing();}
      Motors.Motorstop();
      BarrierCalculate();
      OLED.SelectFace(2, SmilingFace, LongFace, NormalFace);
    }
    
  }
  
}

