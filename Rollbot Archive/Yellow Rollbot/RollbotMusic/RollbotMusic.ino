/* -----------------------------------------------------------------------------
* Author             : Allen
* Check              : Amy
* Version            : V1.0
* Date               : 17/06/2016
* Description        : OLED display control - the buzzer on the car can beeps as controlled;
                       you can even compose and let it make the sounds when the car's running
                       You can find the numbered musical notations of more songs on the Internet. 
                       For example, the notations for the For Alice start as: 
                       323237216; 1367; 3571; 
                       3323237216; 1367; 3176
* Company website    : http://www.sunfounder.com
* 
* ----------------------------------------------------------------------------*/
 
/* Include -------------------------------------------------------------------*/
#include "Rollbot.h"

/* Define -------------------------------------------------------------------*/
RollbotBuzzer Music(9);

/*
 - Scale
* ---------------------------------------------------------------------------*/
int tune[] = 
{
  NOTE_CH3,NOTE_CH2,NOTE_CH3,NOTE_CH2,NOTE_CH3,NOTE_C7,NOTE_CH2,NOTE_CH1,NOTE_C6,
  NOTE_C1,NOTE_C3,NOTE_C6,NOTE_C7,
  NOTE_C3,NOTE_C5,NOTE_C7,NOTE_CH1,
  NOTE_C3,NOTE_CH3,NOTE_CH2,NOTE_CH3,NOTE_CH2,NOTE_CH3,NOTE_C7,NOTE_CH2,NOTE_CH1,NOTE_C6,
  NOTE_C1,NOTE_C3,NOTE_C6,NOTE_C7,
  NOTE_C3,NOTE_CH1,NOTE_C7,NOTE_C6,
};

/*
 - Frequency
* ---------------------------------------------------------------------------*/
float duration[]=
{
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,1,
  0.5,0.5,0.5,1,
  0.5,0.5,0.5,1,
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,1,
  0.5,0.5,0.5,1,
  0.5,0.5,0.5,1,
};

/*
 - setup function
 * ---------------------------------------------------------------------------*/
void setup() 
{  
}

/*
 - loop function
 * ---------------------------------------------------------------------------*/
void loop()
{    
  //Play music
    int Length;
    Length = sizeof(tune)/sizeof(tune[0]);
    Music.MiniBuzzer(tune,duration,Length);
    delay(5000);
}

