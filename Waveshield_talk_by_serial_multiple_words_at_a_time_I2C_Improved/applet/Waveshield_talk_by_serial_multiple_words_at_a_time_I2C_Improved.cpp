#include "SdReader.h"
#include "FatReader.h"
#include "WaveHC.h"
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include <Wire.h>

#include "WProgram.h"
void setup();
void loop();
void add_and_play();
void ls();
void playcomplete(char *name);
void playfile(char *name);
void clearfilename();
char filename[20];
char transitchar;

int wordcount = 1;
int i = 0;

SdReader card;
FatVolume vol;
FatReader root;
FatReader f;
WaveHC wave;      // only one!

#define redled 9

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Wave Test!");
  Wire.begin(); // join i2c bus (address optional for master)

  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(redled, OUTPUT);
  
  if (!card.init()) {
    putstring_nl("Card init. failed!"); return;
  }
  if (!vol.init(card)) {
    putstring_nl("No partition!"); return;
  }
  if (!root.openRoot(vol)) {
    putstring_nl("Couldn't open dir"); return;
  }

  putstring_nl("Files found:");
  ls();

  Serial.println("Test Done.");
}

//unsigned long digit = 0;

void loop() { 
  if (Serial.available() > 0) {
    delay(50);
    while (Serial.available() > 0) {
      transitchar = Serial.read();
      if (transitchar != ' ' && transitchar != ';' && transitchar != ',' && transitchar != '.') { //if char is anything but space or ;. then...
      filename[i] = transitchar; //put char in string
      i++;
      } else if (transitchar == ';') {
        filename[i] = transitchar;
        Wire.beginTransmission(2);
        delay(5); // transmit to device #2
        Wire.send(filename);        // sends command.
        Wire.endTransmission();
        clearfilename(); //clear name
        i = 0; //reset
      }//end else if ;
      else if (transitchar == ',') {
        add_and_play();
        delay(50);
      }//end else if ,
      else if (transitchar == '.') {
        add_and_play();
        delay(100);
      }//end else if .
       else if (strcmp(filename, "ls")  == 0) { //check if the word is ls
       ls();
      clearfilename(); //clear name
      i = 0; //reset
      } else { //if string is not ls then play
    add_and_play();
        } //end of else statement
      }//end of while serial.available
      
      if (strcmp(filename, "ls")  == 0) { //if the word is ls
       ls(); 
       clearfilename();
       i = 0; //reset.
      } else { //if not then play!
      add_and_play();
      } //end of else
    }// end of if serial.available
      
     
  
}//end of void loop


// Subroutines!

/*
 * print dir name field
 * 
 */ 
 void add_and_play() {
  filename[i] = '.'; //add .wav to the words so they can be played
    filename[i+1] = 'W';
    filename[i+2] = 'A';
    filename[i+3] = 'V';
    filename[i+4] = 0;
    playcomplete(filename); //play.
    clearfilename(); //clear
    i = 0; //reset 
 }
 
void printName(dir_t &dir) {
  for (uint8_t i = 0; i < 11; i++) {
    if (dir.name[i] == ' ')continue;
    if (i == 8) Serial.print('.');
    Serial.print(dir.name[i]);
  }
  if (DIR_IS_SUBDIR(dir)) Serial.print('/');
}

void ls() {
  dir_t d;
  root.rewind();
  while (root.readDir(d) > 0) {
    printName(d);
    Serial.println();
  }
}

void playcomplete(char *name) {
  playfile(name);
  if (wave.isplaying) {// already playing something,
    Serial.print("playing: ");
    Serial.println(filename);
  }
  
  while (wave.isplaying);
}

void playfile(char *name) {
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.println(name); return;
  }
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  // ok time to play!
  wave.play();
}

void clearfilename() {
  for (int i; i < 20; i++) {
   filename[i] = '\0'; 
  }
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

