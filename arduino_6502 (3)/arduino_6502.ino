#include "Arduino.h"
#include <util/delay.h>
#include "display.h"
#include "keyboard.h"
#include <EEPROM.h>

#define CH2_SPEAKERPIN 9

uint8_t curkey = 0;
char moveBuffer[11];
char threeHex[3][2];
int hexNum=1;
const int yAxis = A2; 

extern uint8_t RAM[768]; // makes RAM of 6502 unit accessible here

extern "C" 
{
  uint16_t getpc();
  uint8_t getop();
  void exec6502(int32_t tickcount);
  void reset6502();
  
   void serout(uint8_t val) {
    int i;
    
    Serial.write(val);
    
    for (i=1;i<=10;i++)
      moveBuffer[i-1]=moveBuffer[i];
    moveBuffer[10]=val;
    
    if (val=='?') // this indicates microchess is ready for input, so read the 3 hex digits
    {
        threeHex[0][0]=moveBuffer[0];
        threeHex[0][1]=moveBuffer[1];
        threeHex[1][0]=moveBuffer[3];
        threeHex[1][1]=moveBuffer[4];
        threeHex[2][0]=moveBuffer[6];
        threeHex[2][1]=moveBuffer[7];
    }    
    
    if (val=='.') // this indicates microchess is still thinking, tell the CH2 user
    {
     ledOn();
//     tone(9, 698,1); // 523=NOTE_F5
delay(1);
     ledOff();
    }
  }
  
  uint8_t getkey() {
    return(curkey);
  }  
  uint8_t keyready() {
    return (curkey==0?1:0);
  }
    void clearkey() {
    curkey = 0;
  }
  void printhex(uint16_t val) {
    Serial.print(val, HEX);
    Serial.println();
  }

  uint16_t readJoystickY(void)   {
    return (uint16_t) analogRead(yAxis); 
  }

}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


// Below information is just FYI, not really used --
// Block data: Microchess data tables that go into zero page of RAM, see manual
//BLOCK DATA original address relocated address
// .SETW    0070 $c514
// .MOVEX   0090 $c534
// .POINTS  00A0 $c545 
// .OPNING  00C0 $c555
// ------------------------------------------------------------------------

void loadOrSaveBoard(uint8_t curkeyLocal)
{
  int EEPROMaddress;
  int i, slot;

  Serial.println("Entering LS");
  Serial.print(curkeyLocal, DEC);
    
    Serial.println();
    if (curkeyLocal=='L') Serial.print("Load ");
    else Serial.print ("Save ");
    Serial.println("board: enter one digit for slot, followed by return or #");

    // get the slot number (0-9, although actually, we've got plenty more space if you want)
    do {
      _put2char(curkeyLocal, curkeyLocal);	showMatrix(1);
      if (Serial.available()) //Serial port input?
        curkey = Serial.read() & 0x7F;
      if (keyPressed()!=0)   //CH2 shield input?
      {  ledOn(); curkey = getChar();  ledOff(); }
    } while (curkey==curkeyLocal);
   
    slot = curkey;
    Serial.print(slot, DEC);
    Serial.println(" OK... press return or #");
        
    
    // get the hash sign. Anything else than a has means cancel
    do {
      _put2char(curkeyLocal, slot);	showMatrix(1);
      if (Serial.available()) //Serial port input?
        curkey = Serial.read() & 0x7F;
      if (keyPressed()!=0)   //CH2 shield input?
      {  ledOn(); curkey = getChar();  ledOff(); }
    } while (curkey==slot);

    slot = slot - '0'; // convert ascii to int

Serial.print(slot, DEC);
Serial.print(curkey, DEC);

    if (curkey != 13) return;   // # is converted to return 
    if (slot<0) return;
    if (slot>9) return;

    Serial.print(slot, DEC);
          
    curkey = 'V'; // this will go into microchess, 'V' is just a harmless key to press.       
 
  // load or save the 16 bytes into place
  EEPROMaddress = slot * 0x20; // 32 bytes is size of saved board.
  
  for (i==0;i<=31;i++) {
    serout('|');
    if (curkeyLocal == 'S')
    { EEPROM.write(EEPROMaddress+i, RAM[0x50 +i]);
      printhex( (uint16_t) RAM[0x50 +i]); 
    }   
    else  
    { RAM[0x50 +i] = EEPROM.read(EEPROMaddress+i);
      printhex( (uint16_t) RAM[0x50 +i]);
    }
  }
  
  Serial.println("Done.");
  Serial.println();
}
// ------------------------------------------------------------------------


void setup () {
  Serial.begin (9600);
  Serial.println ();

  pinMode(5, OUTPUT);      // sets the digital pin as output
  initDisplay(); // CH2 shield led matrix
  
  reset6502();
 
  Serial.print("freeMemory()="); // just a little check, to avoid running out of RAM!
  Serial.println(freeRam());
  
  getChar();
}

void loop () {
int y;
uint8_t slot;

  exec6502(100); //if timing is enabled, this value is in 6502 clock ticks. otherwise, simply instruction count.

  if (Serial.available()) //Serial port input?
    curkey = Serial.read() & 0x7F;
  
  if (keyPressed()!=0) //CH2 shield input?
  {  ledOn();
     tone(9, 523,1); // 523=NOTE_C5
     curkey = getChar();
     ledOff();
  }
  
  // special case: intercept board (L)oad and (S)ave commands
  // they mean nothing to microchess itself, but are handled here in the emulator
  if ((curkey=='L')|(curkey=='S'))
    loadOrSaveBoard(curkey);

  y = analogRead(yAxis); // Joystick selects one of 3 hex digits to show
  if (y<350) hexNum =2;
  else if (y>650) hexNum=0;
  else hexNum=1;


}

