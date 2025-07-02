/*
    Description: 
    Use ATOM SPK Compile and play PCM files
*/

/*
 * Wav files found here:
 * http://www.galaxyfaraway.com/gfa/1998/12/star-wars-sounds-archive/#Effects
 * 
 * used the command line xxd to convert the wavs to a c array
 * had to slightly modify the c array with sizes (found at bottom of the file)
 * used audacity to convert the mp3 to wav,and change bit rate for smaller
 * files
 * 
 * IE: xxd -i saberup.wav saberup.c
 * 
 * Size of the files is important, too large and compile errors.
 * AtomSPK.h has two spots where bit rates can be changed to match your setting.
 * 8000 makes a small file, but doesn't sound great.
 * 16000 is probably good, but didn't try it. 
 * 
 * This sketch uses 8000 as the bit rate. Good Luck.
 * 
 * the information about xxd was found here: https://github.com/baldram/ESP_VS1053_Library/issues/18
 * 
 */


#include "src/M5Atom/M5Atom.h"
#include "AtomSPK.h"

extern const unsigned char saberdown_wav[10220];
extern const unsigned char saberup_wav[10222];

ATOMSPK _AtomSPK;

void setup(){
    M5.begin(true,false,true);
    _AtomSPK.begin();
    M5.dis.drawpix(0, 0xf00000);
    _AtomSPK.playBeep();
    
}

void loop(){

    if(M5.Btn.isPressed()){
         _AtomSPK.playRAW(saberdown_wav, sizeof(saberdown_wav),true,false);
         _AtomSPK.playRAW(saberup_wav, sizeof(saberup_wav),true,false);
    } 
  
    M5.update();
}
