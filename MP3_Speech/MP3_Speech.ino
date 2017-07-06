
#include <Arduino.h>
#include <avr/io.h>
#include "config.h"
#include "player.h"
#include "vs10xx.h"

#include <Fat16.h>
#include <Fat16Util.h>
#include "newSDLib.h"

int song;
int isplaying;

void setup()
{
  Serial.begin(9600);
  
  InitSPI();
  InitIOForVs10xx();
  InitIOForKeys();
  InitIOForLEDs();
  Mp3Reset();
  if (!initialSDCard())
    return;

  RED_LED_ON();//light red LED


}


void loop()
{ 
  playFile("the.wav");
  playFile("time.wav");
  playFile("is.wav");
  playFile("one.wav");
  playFile("thirty.wav");
  playFile("five.wav");
  playFile("a.wav");
  playFile("m.wav");
  delay(1000);
  playFile("one.wav");
  playFile("two.wav");
  playFile("three.wav");
  playFile("four.wav");
  playFile("five.wav");
  playFile("six.wav");
  playFile("seven.wav");
  playFile("eight.wav");
  playFile("nine.wav");
  playFile("ten.wav");
  playFile("eleven.wav");
  playFile("twelve.wav");
  playFile("thirteen.wav");
  playFile("four.wav");
  playFile("teen.wav");
  delay(1000);
  playFile("twenty.wav");
  playFile("twenty.wav");
  playFile("one.wav");
  playFile("twenty.wav");
  playFile("two.wav");
  delay(1000);
  playFile("one.wav");
  playFile("hundred.wav");
  playFile("fifthy.wav");
  playFile("seven.wav");
  delay(1000);
}


