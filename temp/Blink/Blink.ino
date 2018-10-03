/*
 * Make the builtin LED blink.
 *
 * Copyright (C) 2018 Andreas Motzek andreas-motzek@t-online.de
 *
 * You can use, redistribute and/or modify this file under the terms of the Modified Artistic License.
 * See http://simplysomethings.de/open+source/modified+artistic+license.html for details.
 *
 * This file is distributed in the hope that it will be useful, but without any warranty; without even
 * the implied warranty of merchantability or fitness for a particular purpose.
 */

#include <CooperativeMultitasking.h>
#include "musical_notes.h"

int speakerPin = 0; // speaker connected to digital pin 9 
      
CooperativeMultitasking tasks;
Continuation on; // forward declaration
Continuation off; // forward declaration
Continuation sounds;

void setup() {
  pinMode(12, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  tasks.now(on); // call on() now
  //tasks.now(sounds);
}

void loop() {
   scale();
 r2D2();
 delay(500);
 waka();
 delay(500);
  tasks.run();
}

void on() {
  digitalWrite(12, HIGH);
  tasks.after(1000, off); // call off() in 1000 ms
}

void off() {
  digitalWrite(12, LOW);
  tasks.after(1000, on); // call on() in 1000 ms
}

void sounds() {
  scale();
 r2D2();
 delay(500);
 waka();
 delay(500);
 tasks.after(500,sounds);
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

