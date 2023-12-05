/*
Copyright (c) 2018 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/
/*
 * Adapted from Arduino Mario Bros Tunes with Piezo Buzzer by LeRoy Miller (Sep 18, 2018) 
 * by: Dipto Pratyaksa
 * 31/3/13
 * http://www.princetronics.com/supermariothemesong/
 */

const float  note_C0 = 16.35;  //C0
const float  note_Db0 = 17.32;  //C#0/Db0
const float  note_D0 = 18.35;  //D0
const float  note_Eb0 = 19.45;  //D#0/Eb0
const float  note_E0 = 20.6;  //E0
const float  note_F0 = 21.83;  //F0
const float  note_Gb0 = 23.12;  //F#0/Gb0
const float  note_G0 = 24.5;  //G0
const float  note_Ab0 = 25.96;  //G#0/Ab0
const float  note_A0 = 27.5;  //A0
const float  note_Bb0 = 29.14;  //A#0/Bb0
const float  note_B0 = 30.87;  //B0
const float  note_C1 = 32.7;  //C1
const float  note_Db1 = 34.65;  //C#1/Db1
const float  note_D1 = 36.71;  //D1
const float  note_Eb1 = 38.89;  //D#1/Eb1
const float  note_E1 = 41.2;  //E1
const float  note_F1 = 43.65;  //F1
const float  note_Gb1 = 46.25;  //F#1/Gb1
const float  note_G1 = 49;  //G1
const float  note_Ab1 = 51.91;  //G#1/Ab1
const float  note_A1 = 55;  //A1
const float  note_Bb1 = 58.27;  //A#1/Bb1
const float  note_B1 = 61.74;  //B1
const float  note_C2 = 65.41;  //C2 (Middle C)
const float  note_Db2 = 69.3;  //C#2/Db2
const float  note_D2 = 73.42;  //D2
const float  note_Eb2 = 77.78;  //D#2/Eb2
const float  note_E2 = 82.41;  //E2
const float  note_F2 = 87.31;  //F2
const float  note_Gb2 = 92.5;  //F#2/Gb2
const float  note_G2 = 98;  //G2
const float  note_Ab2 = 103.83;  //G#2/Ab2
const float  note_A2 = 110;  //A2
const float  note_Bb2 = 116.54;  //A#2/Bb2
const float  note_B2 = 123.47;  //B2
const float  note_C3 = 130.81;  //C3
const float  note_Db3 = 138.59;  //C#3/Db3
const float  note_D3 = 146.83;  //D3
const float  note_Eb3 = 155.56;  //D#3/Eb3
const float  note_E3 = 164.81;  //E3
const float  note_F3 = 174.61;  //F3
const float  note_Gb3 = 185;  //F#3/Gb3
const float  note_G3 = 196;  //G3
const float  note_Ab3 = 207.65;  //G#3/Ab3
const float  note_A3 = 220;  //A3
const float  note_Bb3 = 233.08;  //A#3/Bb3
const float  note_B3 = 246.94;  //B3
const float  note_C4 = 261.63;  //C4
const float  note_Db4 = 277.18;  //C#4/Db4
const float  note_D4 = 293.66;  //D4
const float  note_Eb4 = 311.13;  //D#4/Eb4
const float  note_E4 = 329.63;  //E4
const float  note_F4 = 349.23;  //F4
const float  note_Gb4 = 369.99;  //F#4/Gb4
const float  note_G4 = 392;  //G4
const float  note_Ab4 = 415.3;  //G#4/Ab4
const float  note_A4 = 440;  //A4
const float  note_Bb4 = 466.16;  //A#4/Bb4
const float  note_B4 = 493.88;  //B4
const float  note_C5 = 523.25;  //C5
const float  note_Db5 = 554.37;  //C#5/Db5
const float  note_D5 = 587.33;  //D5
const float  note_Eb5 = 622.25;  //D#5/Eb5
const float  note_E5 = 659.26;  //E5
const float  note_F5 = 698.46;  //F5
const float  note_Gb5 = 739.99;  //F#5/Gb5
const float  note_G5 = 783.99;  //G5
const float  note_Ab5 = 830.61;  //G#5/Ab5
const float  note_A5 = 880;  //A5
const float  note_Bb5 = 932.33;  //A#5/Bb5
const float  note_B5 = 987.77;  //B5
const float  note_C6 = 1046.5;  //C6
const float  note_Db6 = 1108.73;  //C#6/Db6
const float  note_D6 = 1174.66;  //D6
const float  note_Eb6 = 1244.51;  //D#6/Eb6
const float  note_E6 = 1318.51;  //E6
const float  note_F6 = 1396.91;  //F6
const float  note_Gb6 = 1479.98;  //F#6/Gb6
const float  note_G6 = 1567.98;  //G6
const float  note_Ab6 = 1661.22;  //G#6/Ab6
const float  note_A6 = 1760;  //A6
const float  note_Bb6 = 1864.66;  //A#6/Bb6
const float  note_B6 = 1975.53;  //B6
const float  note_C7 = 2093;  //C7
const float  note_Db7 = 2217.46;  //C#7/Db7
const float  note_D7 = 2349.32;  //D7
const float  note_Eb7 = 2489.02;  //D#7/Eb7
const float  note_E7 = 2637.02;  //E7
const float  note_F7 = 2793.83;  //F7
const float  note_Gb7 = 2959.96;  //F#7/Gb7
const float  note_G7 = 3135.96;  //G7
const float  note_Ab7 = 3322.44;  //G#7/Ab7
const float  note_A7 = 3520;  //A7
const float  note_Bb7 = 3729.31;  //A#7/Bb7
const float  note_B7 = 3951.07;  //B7
const float  note_C8 = 4186.01;  //C8
const float  note_Db8 = 4434.92;  //C#8/Db8
const float  note_D8 = 4698.64;  //D8
const float  note_Eb8 = 4978.03;  //D#8/Eb8
const float  note_Cb4 = 277.0;
const float  note_Fb4 = 370.0;

#define BUZZER 0

int melody[] = {
  note_E7, note_E7, 0, note_E7,
  0, note_C7, note_E7, 0,
  note_G7, 0, 0,  0,
  note_G6, 0, 0, 0,

  note_C7, 0, 0, note_G6,
  0, 0, note_E6, 0,
  0, note_A6, 0, note_B6,
  0, note_Ab6, note_A6, 0,

  note_G6, note_E7, note_G7,
  note_A7, 0, note_F7, note_G7,
  0, note_E7, 0, note_C7,
  note_D7, note_B6, 0, 0,

  note_C7, 0, 0, note_G6,
  0, 0, note_E6, 0,
  0, note_A6, 0, note_B6,
  0, note_Ab6, note_A6, 0,

  note_G6, note_E7, note_G7,
  note_A7, 0, note_F7, note_G7,
  0, note_E7, 0, note_C7,
  note_D7, note_B6, 0, 0
};

//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

//Underworld melody
int underworld_melody[] = {
  note_C4, note_C5, note_A3, note_A4,
  note_Ab3, note_Ab4, 0,
  0,
  note_C4, note_C5, note_A3, note_A4,
  note_Ab3, note_Ab4, 0,
  0,
  note_F3, note_F4, note_D3, note_D4,
  note_Db3, note_Db4, 0,
  0,
  note_F3, note_F4, note_D3, note_D4,
  note_Db3, note_Db4, 0,
  0, note_Db4, note_Cb4, note_D4,
  note_Cb4, note_Db4,
  note_Db4, note_Gb3,
  note_G3, note_Cb4,
  note_C4, note_Fb4, note_F4, note_E3, note_Ab4, note_A4,
  note_Gb4, note_Db4, note_B3,
  note_Ab3, note_A3, note_Gb3,
  0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

int song = 0;

void setup() {
pinMode(BUZZER, OUTPUT);
Serial.begin(9600);
delay(1000);
}

void loop() {


sing(1);
sing(1);
sing(2);
while(1);
}

void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 2) {
    Serial.println(" 'Underworld Theme'");
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];

      beep(underworld_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      beep(0, noteDuration);

    }

  } else {

    Serial.println(" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];

      beep(melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      beep(0, noteDuration);

    }
  }
}



void beep (float noteFrequency, long noteDuration)
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
              digitalWrite(BUZZER,HIGH);
              delayMicroseconds(microsecondsPerWave);
              digitalWrite(BUZZER,LOW);
              delayMicroseconds(microsecondsPerWave);
          }

}     
