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
//#include <MsTimer2.h>
#include "Rollbot.h"
#include "OLEDData.h"


/* PD Adjustment -----------------------------------------------*/
//#define PD_ADJUST  //uncomment this only to adjust the values of PD

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

int Speed_Dir = 1;

int BASE_SPEED = 50;
int MAX_SPEED = 180;
int MIN_SPEED = 0;
int Speed_L, Speed_R;
float P = 50, D = 1;
int Motorpeed = 0;
int LastError = 0;
int SignalValue = 0;
int PowerFlag = 0;
int Turn_Flag = 0;

void linefollowing();
void PDadjust();
void Turn_Left(int Speed);
void Turn_Right(int Speed);
void Turn_Back(int Speed);
void MazeFollowing();
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

  pinMode(0, INPUT);
  randomSeed(analogRead(0));
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
      Sensor.ThresholdCheck(Speed_Dir,130);
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
    Buttons.ButtonNewMaze(StartDebug,BeeOne);
  }

  //Automatically detect the reference value of sensors, and then start to follow lines

  if (Buttons.Time > 5)
  {
    Buttons.Time = 0;
    ButtonTime = 0;
    if (Times == 1)
    {
      Times--;
      Sensor.ThresholdCheck(Speed_Dir,130);
      Serial.print("threshold:");
      Serial.println(Sensor.threshold);  // 130~160最好
    }
    while (1)
    {
      //Display power supply
      PowerFlag = analogRead(A6);
      OLED.PowerDisplay(PowerFlag);
      linefollowing();
      Sensor.Read_Data();
      if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold)))
      {
        linefollowing();
      }
      else
      {
        Motor.Motordrive(Speed_Dir, BASE_SPEED, BASE_SPEED);
        delay(45);
        Sensor.Read_Data();
        if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) //全白掉头
        {
          digitalWrite(LEFT_LED, HIGH);
          digitalWrite(RIGHT_LED, HIGH);
          for (int i = BASE_SPEED; i > 0; i --)
          {
            Motor.Motordrive(Speed_Dir, i, i); delay(2);
          }
          //MazeFollowing();
          Sensor.Read_Data();
          if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold))
          {
            Turn_Back(BASE_SPEED);
          }
        }
        else if ((Sensor.data[0] < Sensor.threshold) && (Sensor.data[1] < Sensor.threshold) && (Sensor.data[2] < Sensor.threshold) && (Sensor.data[3] < Sensor.threshold) && (Sensor.data[4] < Sensor.threshold)) //全黑  //到终点停止
        {
          for (int i = BASE_SPEED; i > 0;i --)
          {
            Motor.Motordrive(Speed_Dir, i, i); delay(6);
          }
          //MazeFollowing();
          Sensor.Read_Data();
          Turn_Flag = random(2);
          Serial.print(Turn_Flag);
          if ((Sensor.data[0] < Sensor.threshold) && (Sensor.data[1] < Sensor.threshold) && (Sensor.data[2] < Sensor.threshold) && (Sensor.data[3] < Sensor.threshold) && (Sensor.data[4] < Sensor.threshold))
          {
            digitalWrite(RIGHT_LED, HIGH);
            digitalWrite(LEFT_LED, HIGH);
            Motor.Motordrive(Speed_Dir, 0, 0);
            while (1);
          }
          else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold))
          {
            if (Turn_Flag)
            {
              digitalWrite(RIGHT_LED, LOW);
              digitalWrite(LEFT_LED, HIGH);
              Turn_Left(BASE_SPEED);
            }
            else
            {
              digitalWrite(RIGHT_LED, HIGH);
              digitalWrite(LEFT_LED, LOW);
              Turn_Right(BASE_SPEED);
            }
          }
        }
        else if ((Sensor.data[0] < Sensor.threshold) && (Sensor.data[1] < Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) //左转
        {
          for (int i = BASE_SPEED; i > 0;  i --)
          {
            Motor.Motordrive(Speed_Dir, i, i); delay(7);
          }
          //MazeFollowing();
          Sensor.Read_Data();
          Turn_Flag = random(2);
          Serial.print(Turn_Flag);
          if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold))) // T型路口左转
          {
            if (Turn_Flag)
            {
              digitalWrite(RIGHT_LED, LOW);
              digitalWrite(LEFT_LED, HIGH);
              Turn_Left(BASE_SPEED);
            }
            else
            {
              digitalWrite(RIGHT_LED, LOW);
              digitalWrite(LEFT_LED, LOW);
            }
          }
          else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) // 90度左转
          {
              digitalWrite(RIGHT_LED, LOW);
              digitalWrite(LEFT_LED, HIGH);
              Turn_Left(BASE_SPEED);
          }
        }
        else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[3] < Sensor.threshold) && (Sensor.data[4] < Sensor.threshold)) //右转
        {
          for (int i = BASE_SPEED; i > 0; i --)
          {
            Motor.Motordrive(Speed_Dir, i, i); delay(7);
          }
          //MazeFollowing();
          Sensor.Read_Data();
          Turn_Flag = random(2);
          Serial.print(Turn_Flag);
          if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[1] > Sensor.threshold) && (Sensor.data[2] > Sensor.threshold) && (Sensor.data[3] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold)) //90度右转
          {
              digitalWrite(LEFT_LED, LOW);
              digitalWrite(RIGHT_LED, HIGH);
              Turn_Right(BASE_SPEED);
          }
          else if ((Sensor.data[0] > Sensor.threshold) && (Sensor.data[4] > Sensor.threshold) && ((Sensor.data[1] < Sensor.threshold) || (Sensor.data[2] < Sensor.threshold) || (Sensor.data[3] < Sensor.threshold))) //T型路口直走
          {
            if (Turn_Flag)
            {
              digitalWrite(LEFT_LED, LOW);
              digitalWrite(RIGHT_LED, HIGH);
              Turn_Right(BASE_SPEED);
            }
            else
            {
              digitalWrite(LEFT_LED, LOW);
              digitalWrite(RIGHT_LED, LOW);
            }
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
  //PD adjustment
  SignalValue = Sensor.Read_MazeFlag();
  if (SignalValue == 0)
  {
    digitalWrite(LEFT_LED, LOW); digitalWrite(RIGHT_LED, LOW);
    Motor.Motordrive(Speed_Dir, BASE_SPEED, BASE_SPEED);
  }
  else
  {
    if (SignalValue > 0)
    {
      digitalWrite(LEFT_LED, LOW);
      digitalWrite(RIGHT_LED, HIGH);
    }
    if (SignalValue < 0)
    {
      digitalWrite(LEFT_LED, HIGH);
      digitalWrite(RIGHT_LED, LOW);
    }
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
void MazeFollowing()
{
    static int BeeTimes = 0;
   BeeTimes++;
  if(BeeTimes == 1)
  {
    OLED.SelectBee(2,BeeOne,BeeTwo,BeeThree,BeeFour);
  }
  else if(BeeTimes == 2)
  {
    OLED.SelectBee(3,BeeOne,BeeTwo,BeeThree,BeeFour);
  }  
  else if(BeeTimes == 3)
  {
    OLED.SelectBee(4,BeeOne,BeeTwo,BeeThree,BeeFour);
  }  
  else if(BeeTimes == 4)
  {
    BeeTimes = 0;
    OLED.SelectBee(1,BeeOne,BeeTwo,BeeThree,BeeFour);
  } 
}
void Turn_Left(int Speed)
{
  Motor.Motordrive(Speed_Dir, -(Speed + 30) , Speed + 30);
  Sensor.Read_Data();
  while (Sensor.data[0] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motordrive(Speed_Dir, -(Speed - 10), Speed - 10);
  Sensor.Read_Data();
  while (Sensor.data[2] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motorstop();
}
void Turn_Right(int Speed)
{
  Motor.Motordrive(Speed_Dir, Speed + 30, -(Speed + 30));
  Sensor.Read_Data();
  while (Sensor.data[4] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motordrive(Speed_Dir, Speed - 10, -(Speed - 10));
  Sensor.Read_Data();
  while (Sensor.data[2] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motorstop();
}
void Turn_Back(int Speed)
{
  Motor.Motordrive(Speed_Dir, -(Speed + 30), Speed + 30);
  Sensor.Read_Data();
  while (Sensor.data[0] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motordrive(Speed_Dir, -Speed, Speed);
  Sensor.Read_Data();
  while (Sensor.data[2] > Sensor.threshold)
  {
    Sensor.Read_Data();
  }
  Motor.Motorstop();
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

