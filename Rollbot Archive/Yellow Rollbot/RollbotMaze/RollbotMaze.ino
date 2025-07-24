/* -----------------------------------------------------------------------------
  Author             : Allen
  Check              : Amy
  Version            : V1.0
  Date               : 17/06/2016
  Description        : Rollbot follows Maze
  Company website    : http://www.sunfounder.com
   ---------------------------------------------------------------------------*/

/* Include ------------------------------------------------------------------*/
#include <Wire.h>
#include "Rollbot.h"
#include "OLEDData.h"

/* PD Adjustment -----------------------------------------------*/
#define PD_ADJUST  //uncomment this only to adjust the values of PD

/* Print sensors' value by serial monitor -----------------------------------------------*/
//#define Print_Data  //
//#define Print_Flag  //

RollbotReadSensor Sensor;
RollbotOLED OLED;
RollbotLED  LED;
RollbotButton Buttons(10);
RollbotMotors Motor;

#define   LEFT_LED      3
#define   RIGHT_LED     2

int Speed_Dir = 0;
int BASE_SPEED = 150;
int MAX_SPEED = 200;
int MIN_SPEED = 0;
int Speed_L, Speed_R;
float P = 20, D = 0.4;
int Motorpeed = 0;
int LastError = 0;
int SignalValue = 0;
int PowerFlag = 0;

int Route_FlagNum = 0;
int Route_NewNum = 0;
char Route_Flag[100] = "";

void linefollowing();
void PDadjust();
void Turn_Left(int Speed);
void Turn_Right(int Speed);
void Turn_Back(int Speed);
void Simple_Route();
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

  PowerFlag = analogRead(A6);
  OLED.PowerDisplay(PowerFlag);
}

/*
  - loop function
   ---------------------------------------------------------------------------*/
void loop()
{
  static int Times = 1;
  static int ButtonTime = 1;
#ifdef PD_ADJUST
  PDadjust();
#endif
#ifdef Print_Data
  while (1)
  {
    Sensor.Data_Print();
  }
#endif
#ifdef Print_Flag
  while (1)
  {
    if (Times == 1)
    {
      Times--;
      Sensor.ThresholdCheck(Speed_Dir, 130);
      Serial.print("threshold:");
      Serial.println(Sensor.threshold);
    }
    SignalValue = Sensor.Read_MazeFlag();
    Serial.print("SignalValue:");
    Serial.println(SignalValue);
  }
#endif

  //Press the button to start the OLED and line following, hold it for a second.

  if (ButtonTime == 1)
  {
    Buttons.ButtonMaze(StartDebug, Maze, New);
  }

  //Automatically detect the reference value of sensors, and then start to follow lines

  if (Buttons.Time > 5)
  {
    Buttons.Time = 0;
    ButtonTime = 0;
    if (Times == 1)
    {
      Times--;
      Sensor.ThresholdCheck(Speed_Dir, 130);
      Serial.print("threshold:");
      Serial.println(Sensor.threshold);
    }

    while (1)
    {
      //Display power supply
      PowerFlag = analogRead(A6);
      OLED.PowerDisplay(PowerFlag);

      Sensor.Read_Data();

      if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold)))
      {
        linefollowing();
      }
      else
      {
        Motor.Motordrive(Speed_Dir, BASE_SPEED, BASE_SPEED+3);
        delay(5);
        Sensor.Read_Data();
        if ((Sensor.data[0] < Sensor.threshold) && (Sensor.data[1] < Sensor.threshold) && (Sensor.data[2] < Sensor.threshold) && (Sensor.data[3] < Sensor.threshold) && (Sensor.data[4] < Sensor.threshold)) //full black
        {
          digitalWrite(LEFT_LED, HIGH);
          digitalWrite(RIGHT_LED, HIGH);
          for (int i = BASE_SPEED; i > 0; i--)
          {
            Motor.Motordrive(Speed_Dir, i, i); delayMicroseconds(1100);
          }
          Sensor.Read_Data();
          if ((Sensor.data[0] < Sensor.threshold) && (Sensor.data[1] < Sensor.threshold) && (Sensor.data[2] < Sensor.threshold) && (Sensor.data[3] < Sensor.threshold) && (Sensor.data[4] < Sensor.threshold)) //To the end stop
          {
            Motor.Motorstop();
            for (int i = 0; i < Route_FlagNum; i++)
            {
              if (Route_Flag[i] == 'L')
                OLED.DisplayMaze(0, L);
              else if (Route_Flag[i] == 'R')
                OLED.DisplayMaze(0, R);
              else if (Route_Flag[i] == 'S')
                OLED.DisplayMaze(0, S);
              else if (Route_Flag[i] == 'B')
                OLED.DisplayMaze(0, B);
              delay(10);
            }
            while (!(Buttons.Time))
            {
              Buttons.ButtonNewMaze(StartDebug,Bee);
            }
            BASE_SPEED = 180;   
            MAX_SPEED = 240;   
            P = 25; D = 1;
            delay(1000);
            while (1)
            {
              Sensor.Read_Data();

              if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold)))
              {
                linefollowing();
              }
              else
              {
                Motor.Motordrive(Speed_Dir, BASE_SPEED, BASE_SPEED);
                delay(3);

                if (Route_Flag[Route_NewNum] == 'L')
                {
                  digitalWrite(RIGHT_LED, LOW);
                  digitalWrite(LEFT_LED, HIGH);
                  for (int i = BASE_SPEED; i > 0; i--)
                  {
                    Motor.Motordrive(Speed_Dir, i, i); delayMicroseconds(500);
                  }
                  Turn_Left(BASE_SPEED-70);
                }
                else if (Route_Flag[Route_NewNum] == 'R')
                {
                  digitalWrite(RIGHT_LED, HIGH);
                  digitalWrite(LEFT_LED, LOW);
                  for (int i = BASE_SPEED; i > 0; i--)
                  {
                    Motor.Motordrive(Speed_Dir, i, i); delayMicroseconds(500);
                  }
                  Turn_Right(BASE_SPEED-70);
                }
                else if (Route_Flag[Route_NewNum] == 'S')
                {
                  digitalWrite(RIGHT_LED, LOW);
                  digitalWrite(LEFT_LED, HIGH);
                  Motor.Motordrive(Speed_Dir, BASE_SPEED, BASE_SPEED);
                  delay(100);
                }
                else if (Route_Flag[Route_NewNum] == 'B')
                {
                  digitalWrite(LEFT_LED, HIGH);
                  digitalWrite(RIGHT_LED, HIGH);
                  for (int i = BASE_SPEED; i > 0; i--)
                  {
                    Motor.Motordrive(Speed_Dir, i, i); delayMicroseconds(500);
                  }
                  Turn_Back(BASE_SPEED-70);
                }
                Route_NewNum ++;
                if (Route_NewNum > Route_FlagNum)
                {
                  digitalWrite(LEFT_LED, HIGH);
                  digitalWrite(RIGHT_LED, HIGH);
                  Motor.Motorstop();
                  while (1);
                }
              }
            }
          }
          else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold))) //Turn left at the crossroads
          {
            Route_Flag[Route_FlagNum] = 'L';
            Route_FlagNum++;
            Simple_Route();
            OLED.DisplayMaze(1, L);
            Turn_Left(BASE_SPEED-40);
          }
          else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) //T-junction turn left
          {
            Route_Flag[Route_FlagNum] = 'L';
            Route_FlagNum++;
            Simple_Route();
            OLED.DisplayMaze(1, L);
            Turn_Left(BASE_SPEED-40);
          }
        }
        else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) //All white turned around
        {
          digitalWrite(LEFT_LED, HIGH);
          digitalWrite(RIGHT_LED, HIGH);
          for (int i = BASE_SPEED; i > 0; i--)
          {
            Motor.Motordrive(Speed_Dir, i, i); delayMicroseconds(1100);
          }
          Sensor.Read_Data();
          if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold))
          {
            Route_Flag[Route_FlagNum] = 'B';
            Route_FlagNum++;
            OLED.DisplayMaze(1, B);
            Turn_Back(BASE_SPEED-30);
          }
        }
        else if ((Sensor.data[0] < Sensor.threshold) && (Sensor.data[1] < Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) //Turn left
        {
          digitalWrite(RIGHT_LED, LOW);
          digitalWrite(LEFT_LED, HIGH);
          for (int i = BASE_SPEED; i > 0; i--)
          {
            Motor.Motordrive(Speed_Dir, i, i); delayMicroseconds(1100);
          }
          Sensor.Read_Data();
          if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold))) // T-junction turn left
          {
            Route_Flag[Route_FlagNum] = 'L';
            Route_FlagNum++;
            Simple_Route();
            OLED.DisplayMaze(1, L);
            Turn_Left(BASE_SPEED-40);
          }
          else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) // Turn left at 90 degrees
          {
            Route_Flag[Route_FlagNum] = 'L';
            Route_FlagNum++;
            OLED.DisplayMaze(1, L);
            Turn_Left(BASE_SPEED-40);
          }
        }
        else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[3] < Sensor.threshold) && (Sensor.data[4] < Sensor.threshold)) //Turn right
        {
          digitalWrite(LEFT_LED, LOW);
          digitalWrite(RIGHT_LED, HIGH);
          for (int i = BASE_SPEED; i > 0; i--)
          {
            Motor.Motordrive(Speed_Dir, i, i); delayMicroseconds(1100);
          }
          Sensor.Read_Data();
          if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold))) //T-type straight walk
          {
            Route_Flag[Route_FlagNum] = 'S';
            Route_FlagNum++;
            OLED.DisplayMaze(1, S);
          }
          else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) //90 degrees turn right
          {
            Route_Flag[Route_FlagNum] = 'R';
            Route_FlagNum++;
            OLED.DisplayMaze(1, R);
            Turn_Right(BASE_SPEED-40);
          }
        }
      }
    }
  }
}

/*
  - line following function
   ---------------------------------------------------------------------------*/
void linefollowing()
{

  //Detect the relative position of the Rollman-mini and black line in real time
  SignalValue = Sensor.Read_MazeFlag();

  digitalWrite(LEFT_LED, LOW); digitalWrite(RIGHT_LED, LOW);
  //PD adjustment
  if (SignalValue == 0)
  {
    Motor.Motordrive(Speed_Dir, BASE_SPEED, BASE_SPEED+3);
  }
  else
  {
    Motorpeed = int(P * SignalValue + D * (SignalValue - LastError));
    LastError = 0;
    LastError = SignalValue;
    Speed_L = BASE_SPEED + Motorpeed;
    Speed_R = BASE_SPEED - Motorpeed;
    if (Speed_R > MAX_SPEED ) Speed_R = MAX_SPEED;
    if (Speed_L > MAX_SPEED )  Speed_L = MAX_SPEED;
    if (Speed_R < MIN_SPEED) Speed_R = MIN_SPEED;
    if (Speed_L < MIN_SPEED)   Speed_L = MIN_SPEED;
    Motor.Motordrive(Speed_Dir, Speed_L, Speed_R);
  }
}
void Turn_Left(int Speed)
{
  Motor.Motordrive(Speed_Dir, -Speed, Speed);
  Sensor.Read_Data();
  while (Sensor.data[0] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motordrive(Speed_Dir, -(Speed-30), Speed-30);
  Sensor.Read_Data();
  while (Sensor.data[2] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motorstop();
}
void Turn_Right(int Speed)
{
  Motor.Motordrive(Speed_Dir, Speed, -Speed);
  Sensor.Read_Data();
  while (Sensor.data[4] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motordrive(Speed_Dir, Speed-30, -(Speed-30));
  Sensor.Read_Data();
  while (Sensor.data[2] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motorstop();
}
void Turn_Back(int Speed)
{
  Motor.Motordrive(Speed_Dir, Speed, -Speed);
  Sensor.Read_Data();
  while (Sensor.data[4] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motordrive(Speed_Dir, Speed-20, -(Speed-20));
  Sensor.Read_Data();
  while (Sensor.data[2] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motorstop();
}

void Simple_Route()
{
  //    if((Route_Flag[Route_FlagNum-1]=='L')&&(Route_Flag[Route_FlagNum-2]=='B')&&(Route_Flag[Route_FlagNum-3]=='L'))
  //    {Route_Flag[Route_FlagNum-3]='S';Route_FlagNum=Route_FlagNum-2;
  //     }
  //     else
  //        return;
  int total_angle = 0;
  int i;
  if ((Route_Flag[Route_FlagNum - 2] == 'B') && (Route_FlagNum > 2))
  {
    for (i = 1; i <= 3; i++)
    {
      switch (Route_Flag[Route_FlagNum - i])
      {
        case 'R':
          total_angle += 90;
          break;
        case 'L':
          total_angle += 270;
          break;
        case 'B':
          total_angle += 180;
          break;
      }
    }
    total_angle = total_angle % 360;
    switch (total_angle)
    {
      case 0:
        Route_Flag[Route_FlagNum - 3] = 'S';
        break;
      case 90:
        Route_Flag[Route_FlagNum - 3] = 'R';
        break;
      case 180:
        Route_Flag[Route_FlagNum - 3] = 'B';
        break;
      case 270:
        Route_Flag[Route_FlagNum - 3] = 'L';
        break;
    }
    Route_FlagNum -= 2;
  }
  else if ((Route_Flag[Route_FlagNum - 3] == 'B') && (Route_FlagNum > 2))
  {
    for (i = 1; i <= 5; i++)
    {
      switch (Route_Flag[Route_FlagNum - i])
      {
        case 'R':
          total_angle += 90;
          break;
        case 'L':
          total_angle += 270;
          break;
        case 'B':
          total_angle += 180;
          break;
      }
    }
    total_angle = total_angle % 360;
    switch (total_angle)
    {
      case 0:
        Route_Flag[Route_FlagNum - 5] = 'S';
        break;
      case 90:
        Route_Flag[Route_FlagNum - 5] = 'R';
        break;
      case 180:
        Route_Flag[Route_FlagNum - 5] = 'B';
        break;
      case 270:
        Route_Flag[Route_FlagNum - 5] = 'L';
        break;
    }
    Route_FlagNum -= 4;
  }
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
    P = P + 0.1;
    Serial.print(P);
  }
  else if (temp == 'd')
  {
    P = P - 0.1;
    Serial.print(P);
  }
  else if (temp == 'a')
  {
    D = D - 0.01;
    Serial.print(D);
  }
  else if (temp == 'b')
  {
    D = D + 0.01;
    Serial.print(D);
  }
}
