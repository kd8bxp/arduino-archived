
void checkDistance() {
 
  if (distance <= 950) {
    musicFlag = 1;
    playMusic(); }
  if (distance >= 951) {
    musicFlag = 0;}
}


/*Portions of code by Tom Igoe (21 Jan 2010/30 Aug 2011) part of the Star Trek DS9 Theme Music
 * modified for use with the M5 Stack. (Aug 11, 2018 LeRoy Miller)
 */
 
void playMusic() {

  if (musicFlag == 0) {
  
int melody[] = { NOTE_G4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_F5, NOTE_E5, NOTE_F5, NOTE_G5};
int noteDurations[] = { 16, 16, 16, 2,4,8,8,2};
for (int thisNote = 0; thisNote < 8/*17*/; thisNote++) {
   int noteDuration = 1800/noteDurations[thisNote];
   M5.Speaker.tone(melody[thisNote]);
   delay(noteDuration);
   M5.Speaker.mute();
   delay(35);
   }
  }
}


