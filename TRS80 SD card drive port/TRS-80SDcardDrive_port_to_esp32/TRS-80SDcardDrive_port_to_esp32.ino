// **************************************************************************
// **** MyTeSed: T100 SD Card Reader for Arduino Nano and SD Card Reader ****
// **************************************************************************
// ** David Stephenson 2017-08-23  **
// ** Version 0.01                 **
// **********************************
#include <SPI.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Wire.h" // alias for `#include "SSD1306Wire.h"`
#include "SH1106.h"
#include <mySD.h>

enum SERIAL_MODE {FREE, COMMAND, DATA_IN, DATA_OUT, FILES_OUT};

const byte LF=10;
const byte CR=13;
const byte EF=26;

SH1106 display(0x3c, 21, 22);

class CardReader {
 private:
  SERIAL_MODE eSerialMode = FREE;
  //DRIVE_COMMAND eDriveMode = NONE;

  File MyFile;
  
  String sFileName;
  String sInString;
  
  unsigned long TimeLastUpdate = 0;
  
  // Class Private Functions
  void LoadBas() {
   
   char cRead, cLast;

   if(eSerialMode == DATA_OUT){
    char* buffer;
    sFileName.toCharArray(buffer,20);
    MyFile = SD.open(buffer);
    display.clear();
  display.drawString(0,0, "LOAD" + sFileName);
  display.display();
    if(MyFile){
     while(MyFile.available()){
      cRead=MyFile.read();
      if (cRead== LF){
       if (cLast != CR){
        Serial.write(CR);
       }
       Serial.write(LF);
      } else {
       Serial.write(cRead);
       cLast = cRead;
      }
     }
    }
    MyFile.close();
    Serial.write(EF);
    eSerialMode = FREE;
   }
  }

  void SaveBas(char cInChar) {
   display.drawString(0,0, "SAVE "+ MyFile);
   display.display();
   if(eSerialMode == DATA_IN){
    if(cInChar != EF){
     MyFile.print(cInChar);
    } else { 
     MyFile.close();
     eSerialMode = FREE;
    }
   }  
  }
  
  void FilesOut() {
   
  }
  
  
  void commandIn(){
   String KEYWORDS[7] = {"LOAD","SAVE","KILL","FILES","MOVE","CP2SD","CP2R"};
   String sInSubString;

   sInString.trim();
  display.clear();
   display.drawString(0,0, sInString);
   display.display();
   //Serial.print(sInString);

   if (sInString.length() >= 3){
    for (byte bKeyword = 0 ; bKeyword < 8 ; bKeyword++){
     sInSubString = sInString.substring(0,KEYWORDS[bKeyword].length() );
     sInSubString.toUpperCase();
     if (sInSubString.indexOf(KEYWORDS[bKeyword])!=-1){
      if (KEYWORDS[bKeyword] == "LOAD") {
       sFileName = sInString.substring(4);
       sFileName.trim();
       delay(500);
       eSerialMode = DATA_OUT;
       LoadBas();
      }

      else if (KEYWORDS[bKeyword] == "SAVE" || KEYWORDS[bKeyword] == "CP2D") {
       sFileName = sInString.substring(4);
       sFileName.trim();
       char* buffer;
    sFileName.toCharArray(buffer,20);
       if (SD.exists(buffer)) {
        SD.remove(buffer);
       }
       MyFile = SD.open(buffer, FILE_WRITE);
       eSerialMode = DATA_IN;
      }
      
      else if (KEYWORDS[bKeyword] == "FILES") {
       eSerialMode = FILES_OUT;
       FilesOut();
      }
     }
    }
   }

   //sInCommand = false;
   sInString = "";
   
  }
 
 public:
 
 void SerialIn(char cInChar){
  switch(eSerialMode) {
   case FREE:
    if(cInChar == CR){
     eSerialMode = COMMAND;
     commandIn();
    } else {
     sInString += cInChar;
    }
    break;
   case DATA_IN:
     SaveBas(cInChar);
    break;
  }
 }

};

CardReader MyCard;

void setup()

{
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  // Open serial communications and wait for port to open:
  Serial.begin(1200);
    //57I1D
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  

 if (!SD.begin(5,23,19,18)) {            //T1:13,15,2,14  T2: 23,5,19,18 M5：4,23,19,18 uint8_t csPin, int8_t mosi, int8_t miso, int8_t sck
    Serial.println("initialization failed!");
    return;
  }
 //Serial.println("mango");
 //Serial.println("mangos");
 display.drawString(0,0,"Ready.");
 display.display();
  }




void loop()
{
 char cInChar;
 if (Serial.available()) {
  cInChar = (char)Serial.read();
  //sInString += cInChar;
  MyCard.SerialIn(cInChar);
  //Serial.write(cInChar);
 }
}
