// **************************************************************************
// **** MyTeSed: T100 SD Card Reader for Arduino Nano and SD Card Reader ****
// **************************************************************************
// ** David Stephenson 2017-08-23  **
// ** Version 0.01                 **
// **********************************
#include <SPI.h>
#include <SD.h>
//#include <SoftwareSerial.h>

enum SERIAL_MODE {FREE, COMMAND, DATA_IN, DATA_OUT, FILES_OUT};

const byte LF=10;
const byte CR=13;
const byte EF=26;

//SoftwareSerial Serial(12, 13); // RX, TX

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
    MyFile = SD.open(sFileName);

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

  void SaveBas() {

  MyFile = SD.open(sFileName, FILE_WRITE);
       eSerialMode = DATA_IN;
   if(eSerialMode == DATA_IN){
     while (Serial.available() >0) {
      
    char cInChar = (char)Serial.read();
    if(cInChar != EF){
     MyFile.print(cInChar);
     
    } //else { 
     }
     MyFile.close();
     eSerialMode = FREE;
    }
   }  
 // }
  //}
   
  void FilesOut(File dir, int numTabs) {

   while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      FilesOut(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
    Serial.write(EF);
    eSerialMode = FREE;
  }
  }
  
  
  void commandIn(){
   String KEYWORDS[7] = {"LOAD","SAVE","KILL","FILES","MOVE","CP2SD","CP2R"};
   String sInSubString;

   sInString.trim();

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
       if (SD.exists(sFileName)) {
        SD.remove(sFileName);
       }
       SaveBas();
      }
      
      else if (KEYWORDS[bKeyword] == "FILES") {
       eSerialMode = FILES_OUT;
       MyFile = SD.open("/");
       FilesOut(MyFile, 0);
       MyFile.close();
      }
      
     }
    }
   }

   //sInCommand = false;
   sInString = "";
   Serial.write(EF);
    eSerialMode = FREE;
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
     SaveBas();
    break;
  }
 }

};

CardReader MyCard;

void setup()

{
  // Open serial communications and wait for port to open:
  Serial.begin(300);
  //300 baud 37i1d slow but works better
    //57I1D 1200 baud
    Serial.setTimeout(3000);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  delay(1000);

  if(!SD.begin(5)){
   Serial.println("fail");
    return;
  } 
 Serial.println("mango");
 Serial.println("mangos");
  
}



void loop()
{
 char cInChar;
 if (Serial.available()) {
  cInChar = (char)Serial.read();
  //sInString += cInChar;
  MyCard.SerialIn(cInChar);
 // Serial.write(cInChar);
 }
}
