/*
    Description: 
    Use ATOM SPK Compile and play PCM files
*/


#include "src/M5Atom/M5Atom.h"
#include "AtomSPK.h"

extern const unsigned char mp3[62282];

ATOMSPK _AtomSPK;

void setup(){
    M5.begin(true,false,true);
    _AtomSPK.begin();
    M5.dis.drawpix(0, 0xf00000);
    _AtomSPK.playBeep();
    
}

void loop(){

    if(M5.Btn.isPressed()){
        while(1) _AtomSPK.playRAW(mp3, sizeof(mp3),true,false);
    }
    M5.update();
}
