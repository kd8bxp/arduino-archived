/* -----------------------------------------------------------------------------
  Author             : Allen
  Check              : Amy
  Version            : V1.0
  Date               : 17/06/2016
  Description        : OLED display control
  Company website    : http://www.sunfounder.com
   ---------------------------------------------------------------------------*/

/* Include -------------------------------------------------------------------*/
#include "Rollbot.h"
#include "OLEDData.h"

/* Define -------------------------------------------------------------------*/
RollbotReadSensor Sensor;
RollbotOLED OLED;
RollbotLED  LED;
RollbotButton Buttons(10);

/*
  - setup function
   ---------------------------------------------------------------------------*/
void setup() {
  //Start serial for debug
  Serial.begin(115200);
  Serial.println("Robot starts initialization");

  //Initialize OLED and LED
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

  //Press the button to start the OLED,hold it for a second.Then you can see five bar graphs that indicate the intensity of the sensors.
  static int ButtonTime = 1;
  if (ButtonTime == 1)
  {
    Buttons.Button(2, SmilingFace, LongFace, NormalFace, StartDebug, Direction, Foward, Left, Right, Back, TimeNumber, SignalNumber);
  }
  if (Buttons.Time > 5)
  {
    ButtonTime = 0;
    Sensor.Read_Data();
    OLED.OLED_SignalDisplay(Sensor.OLED_Flag[0], Sensor.OLED_Flag[1], Sensor.OLED_Flag[2], Sensor.OLED_Flag[3], Sensor.OLED_Flag[4]);
  }
}



