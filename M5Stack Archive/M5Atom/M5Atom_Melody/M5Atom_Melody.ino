#define BUZZER_PIN 23
#define LEDC_CHANNEL 1
#include "src/M5Atom/M5Atom.h"
#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_C4,NOTE_G4, NOTE_G4, NOTE_A4,NOTE_A4, NOTE_G4, 
  NOTE_F4, NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_D4,NOTE_C4, 
  NOTE_G4,NOTE_G4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,
  NOTE_G4,NOTE_G4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,
  NOTE_C4, NOTE_C4,NOTE_G4, NOTE_G4, NOTE_A4,NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_D4,NOTE_C4,};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4,4,4,4,4,4,2,
  4,4,4,4,4,4,2,
  4,4,4,4,4,4,2,
  4,4,4,4,4,4,2,
  4,4,4,4,4,4,2, 
  4,4,4,4,4,4,2,};

void setup()
{
    M5.begin(true, false, true);
    ledcSetup(LEDC_CHANNEL, 12000, 8);
    ledcAttachPin(BUZZER_PIN, 1);
    M5.dis.drawpix(0, CRGB(0, 255, 0));
}

void loop()
{
    M5.update();
    if (M5.Btn.wasPressed())
    {
        M5.dis.drawpix(0, CRGB(0, 0, 255));
        playMusic();
    }
    else
    {
        M5.dis.drawpix(0, CRGB(0, 255, 0));
    }
    delay(100);
}

void playMusic()
{
    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {

        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        ledcWriteTone(LEDC_CHANNEL, melody[thisNote]);
        delay(noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        ledcWriteTone(LEDC_CHANNEL, 0);
    }
}
