#include <Arduino.h>
#include "sound.h"

void sound_play (int noteFrequency)
{
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many milliseconds there are per HIGH/LOW cycles.
  float millisecondsPerCycle = 100000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond

  for (x=0;x<millisecondsPerCycle;x++)
  {
    digitalWrite(SOUND_PIN,HIGH);
    delayMicroseconds(microsecondsPerWave);
    digitalWrite(SOUND_PIN,LOW);
    delayMicroseconds(microsecondsPerWave);
  }
}

void sound_hit() {
   sound_play(3951);
   sound_play(988);
}

void sound_fire()
{
   sound_play(16);
}

void sound_kill()
{
   sound_play(300);
}

void sound_item()
{
   sound_play(988);
   sound_play(3951);
}
