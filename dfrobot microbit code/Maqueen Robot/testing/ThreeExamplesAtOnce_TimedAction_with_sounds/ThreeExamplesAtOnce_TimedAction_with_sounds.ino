/*
||
|| @file ThreeExamplesAtOnce.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | This sketch blinks an LED as [url=http://arduino.cc/en/Tutorial/Blink]Examples->Digital->Blink[/url] 
|| |             sets a led on or off according to serial buffer as [url=http://arduino.cc/en/Tutorial/PhysicalPixel]Examples->Digital->PhysicalPixel[/url] 
|| |             prints the ascii table as [url=http://arduino.cc/en/Tutorial/ASCIITable]Examples->Digital->ASCIITable[/url] 
|| #
||
*/

#include <TimedAction.h>
#include "musical_notes.h"

int speakerPin = 0; // speaker connected to digital pin 9 
      
void blink();
void physicalPixel();
void asciiTable();

//this initializes a TimedAction object that will change the state of an LED every second.
TimedAction blinkAction 		=	TimedAction(500,blink);
//this initializes a TimedAction object that will change the state of an LED 
//according to the serial buffer contents, every 50 milliseconds
TimedAction physicalPixelAction = 	TimedAction(500,physicalPixel);
//this initializes a TimedAction object that will write tha ascii table to the serial every ten seconds
TimedAction asciiTableAction 	= 	TimedAction(1000,asciiTable); 

//pin / state variables
#define ledPin 12
#define physicalPin 8
boolean ledState = false;
boolean led2State = true;


void setup(){
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,ledState);
  pinMode(physicalPin, OUTPUT);
  pinMode(speakerPin, OUTPUT); // sets the speakerPin to be an output 
  Serial.begin(9600);
}

void loop(){
  blinkAction.check(); //trigger every second
  physicalPixelAction.check(); //trigger every 50 millisecond
  asciiTableAction.check(); //trigger every 10 second
}

//[url=http://arduino.cc/en/Tutorial/Blink]Examples->Digital->Blink[/url] 
void blink(){
  ledState ? ledState=false : ledState=true;
  digitalWrite(ledPin,ledState);
}

//[url=http://arduino.cc/en/Tutorial/PhysicalPixel]Examples->Digital->PhysicalPixel[/url] 
void physicalPixel()
{
  led2State ? led2State=false : led2State=true;
  digitalWrite(physicalPin,led2State);
}

//[url=http://arduino.cc/en/Tutorial/ASCIITable]Examples->Digital->ASCIITable[/url] 
void asciiTable() 
{ 
 scale();
 //r2D2();
 //delay(500);
 //waka();
 
}

void beep (int speakerPin, float noteFrequency, long noteDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
          } 
}     

void scale() 
{    
          beep(speakerPin, note_C7,50); //C: play the note C for 500ms 
          beep(speakerPin, note_D7,50); //D 
          beep(speakerPin, note_E7,50); //E 
          beep(speakerPin, note_F7,50); //F 
          beep(speakerPin, note_G7,50); //G 
          beep(speakerPin, note_A7,50); //A 
          beep(speakerPin, note_B7,50); //B 
          beep(speakerPin, note_C8,50); //C 
}  

void r2D2(){
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
}

void waka() {
  for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
}
