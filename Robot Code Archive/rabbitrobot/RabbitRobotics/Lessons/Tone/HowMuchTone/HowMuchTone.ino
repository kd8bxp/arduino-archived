/*
  How Much is That Doggie in the Window?

 Plays a melody

 circuit:
 * speaker or Piezo on digital pin 8

 created 07 June 2016
  by Ray Scott

 */
#include "pitches.h"

int SpeakerPin = 8;



void setup() {
  tone(SpeakerPin, NOTE_G4, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_C5, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_G4, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_E4, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_C4, 750); // G4 at 3/8 duration
  delay(750); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_A5, 250); // G4 at 1/8 duration
  delay(250); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_G4, 250); // G4 at 1/4 duration
  delay(250); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_E4, 250); // G4 at 1/4 duration
  delay(250); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_G4, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_D4, 1000); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note

  delay(800);
  // Bark .. Bark
  tone(SpeakerPin, NOTE_G6, 250); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_G6, 250); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note

  delay(600);
  

  tone(SpeakerPin, NOTE_G4, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_C5, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_A5, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_G4, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_F4, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_B5, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_A5, 500); // G4 at 1/4 duration
  delay(500); // pause  for time (mS) between each note
  tone(SpeakerPin, NOTE_G4, 2000); // G4 at 1/1 duration
  delay(2000); // pause  for time (mS) between each note

  
}

void loop() {
  // no need to repeat the melody.
}
