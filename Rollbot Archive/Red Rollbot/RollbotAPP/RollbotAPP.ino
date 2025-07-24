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
RollbotButton Buttons(10);

int Speed_Dir = 0;

int Lspeed_F = 120, Rspeed_F = 120;
int Lspeed_B = -120, Rspeed_B = -120;
int Lspeed_L = 50, Rspeed_L = 120;
int Lspeed_R = 120, Rspeed_R = 50;
/*
  - setup function
  ---------------------------------------------------------------------------*/
void setup() {
  //start serial for debug
  Serial.begin(115200);
  Serial.println("Robot starts initialization");

  //start serial for IIC
  Wire.begin();

  //initialize OLED  and LED
  OLED.OLED_Init();
  OLED.Clear_Screen(0, 0, 128, 8, 0x00);
  OLED.SignalDisplay();
  OLED.PowerDisplay(0);
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
  if (ButtonTime == 1)
  {
    Buttons.Button(1, SmilingFace, LongFace, NormalFace, StartDebug, Direction, Foward, Left, Right, Back, TimeNumber, SignalNumber);
  }
  //APP control
  static char ReceiveByte = 0;
  int DirectionFlag = 0;
  if (Buttons.Time > 5)
  {
    ButtonTime = 0;
    while (Serial.available())
    {
      ReceiveByte = Serial.read();
    }

    //forward
    if (ReceiveByte == 'c')
    { DirectionFlag = 7; Motors.Motordrive(Speed_Dir, Lspeed_F, Rspeed_F);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber);
    }

    //back
    else if (ReceiveByte == 'd')
    { DirectionFlag = 8; Motors.Motordrive(Speed_Dir, Lspeed_B, Rspeed_B);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber);
    }

    //left
    else if (ReceiveByte == 'a')
    { DirectionFlag = 9; Motors.Motordrive(Speed_Dir, Lspeed_L, Rspeed_L);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber);
    }

    //right
    else if (ReceiveByte == 'b')
    { DirectionFlag = 10; Motors.Motordrive(Speed_Dir, Lspeed_R, Rspeed_R);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber);
    }

    //stop
    else if (ReceiveByte == 's')
    {
      ReceiveByte = 0, DirectionFlag = 11;
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber);
      Motors.Motorstop();
    }

    //When dicerolling,the time to go forward
    else if (ReceiveByte == '1')
    { ReceiveByte = 0, DirectionFlag = 1; Motors.Motordrive(Speed_Dir, Lspeed_F, Rspeed_F);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber); Motors.Motorstop();
    }
    else if (ReceiveByte == '2')
    { ReceiveByte = 0, DirectionFlag = 2; Motors.Motordrive(Speed_Dir, Lspeed_F, Rspeed_F);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber); Motors.Motorstop();
    }
    else if (ReceiveByte == '3')
    { ReceiveByte = 0, DirectionFlag = 3; Motors.Motordrive(Speed_Dir, Lspeed_F, Rspeed_F);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber); Motors.Motorstop();
    }
    else if (ReceiveByte == '4')
    { ReceiveByte = 0, DirectionFlag = 4; Motors.Motordrive(Speed_Dir, Lspeed_F, Rspeed_F);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber); Motors.Motorstop();
    }
    else if (ReceiveByte == '5')
    { ReceiveByte = 0, DirectionFlag = 5; Motors.Motordrive(Speed_Dir, Lspeed_F, Rspeed_F);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber); Motors.Motorstop();
    }
    else if (ReceiveByte == '6')
    { ReceiveByte = 0, DirectionFlag = 6; Motors.Motordrive(Speed_Dir, Lspeed_F, Rspeed_F);
      OLED.NumDisplay(DirectionFlag, Direction, Foward, Left, Right, Back, TimeNumber); Motors.Motorstop();
    }
  }
}

