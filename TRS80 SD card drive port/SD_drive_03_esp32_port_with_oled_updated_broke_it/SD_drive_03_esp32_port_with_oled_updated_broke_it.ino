/*
 * WiFi SD card drive
 * Ported to ESP32 by LeRoy Miller May 2019
 * see credit.h tab for additional information and credit to other projects.
 */

#include <SPI.h>
#include <SD.h>
#include "XModem.h" // Xmodem Only working with the NEC PC-8401 at the moment... To be fixed at some point.
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Wire.h" // alias for `#include "SSD1306Wire.h"`
#include "SH1106.h" //https://github.com/ThingPulse/esp8266-oled-ssd1306
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include "defines.h"
//#include "wifimodem.ino" 

SH1106 display(0x3c, 21, 22); 
WiFiClient tcpClient;
WiFiServer tcpServer(tcpServerPort);
WebServer webServer(80);
MDNSResponder mdns;


// **** Setup Constants ****

// Set default Baud Rate
// Settings on T100/T02/T200 for reference
// 1 = 75,   2 = 110,  3 = 300
// 4 = 600,  5 = 1200, 6 = 2400
// 7 = 4800, 8 = 9600, 9 = 19200
//const int BAUD_RATE=19200; // Tandy T100/T02/T200
//const int BAUD_RATE=9600; // NEC PC-8401
const int BAUD_RATE=300;

// Write out Delay
// Without Hardware flow control all writing to the T100 requires a delay
// This allows for fast saving and a normal loading speed
// This ends up reducing the T100 inbound baud rate to aprox 2400 from 19200
// Writing to the Screen in Telcom from SD requires 2*the OUT_DELAY
const byte OUT_DELAY=10;

// Commands issued to MyTeSeD
// LOAD:  Load directly from SD card into BASIC
// SAVE:  Save BASIC File directly to SD card
// KILL:  Delete file from SD card
// MOVE:  Rename File on SD card
// TCOPY: Copy from SD card to T100
// MCOPY: from T100 to SD
// ERR:   Return last Error code.
const String KEYWORDS[15] = {"LOAD","SAVE","FILES","KILL","HELP","DEL","DIR","COPY2R","COPY2D","CAT","XGET","MODEM","WIFI","MDM"};

// Set Serial Port Modes
// FREE:     Not busy
// DATA_IN:  Reading From T100
// DATA_OUT: Sending to T100
enum SerialStates {FREE, DATA_IN, DATA_OUT, MODEM};

// Command Time Out: If no data from T100 after 15 seconds timeout on "SAVE" or "COPY2D"
const long TIMEOUT = 15000;

// Character Codes
// LF = Line Feed, CR = carriage return, EF = End of File
//enum CharCodes {LF = 10, CR = 13, EF = 26};

// ** LF & CR in XModem.h
// #define LF 0x0a
// #define CR 0x0d
#define EF 0x1a

// Error Codes
// Same values as T100
// SN = Syntax Error
// FF = File Not Found
// NM = Bad File Name
// CF = File Not Open (SD Card Not Found)
// Values could be sent to T100 for standard feed back with 'Error X' command
enum ErrorCodes {OK1 = 0, SN = 2 , FF = 52, NM = 55, CF = 58};

// Define Xmodem
XModem MyXmodem(&Serial, ModeXModem);

// **** Define Classes ****


// ***********************************************
// * Read / Writ SD Card send Serial out to T100 *
// ***********************************************
class SDCardDrive {

	private:

	// Variable for Saving Files
	File InFile; // File to save to

	ErrorCodes FileOut(String sFileName, byte bDelayOutLevel = 0){

		ErrorCodes eError = OK1;

		File OutFile;
		char cRead, cLast, cCount=1;
display.clear();
display.drawString(0,0,"Load...");
display.drawString(45,0,sFileName);
display.display();

		delay(250);
		OutFile = SD.open(sFileName);

		if(!OutFile){
			eError = FF;
		} else {
			// If bDelayOutLevel multiplier greater than 1
			// Then we are (willingly) writing to the LCD Screen
			// Add a LF for easy reading
			if(bDelayOutLevel > 1){
				Serial.write(CR);
				Serial.write(LF);
				delay(OUT_DELAY*(bDelayOutLevel*2));
			}

			while(OutFile.available()){
				if ( cCount == 2 ){
					delay(OUT_DELAY*bDelayOutLevel);
      				cCount = 1;
				}
				cCount++;
				cRead=OutFile.read();

				// If This is a BASIC file 'LOAD' ensures CR formating is used
				// Else outputs text as is
				if (cRead == LF && cLast != CR ){
					//Serial.write(cRead);
					Serial.write(CR);
        
				}

				Serial.write(cRead);
				cLast = cRead;
        
			}
		}

		OutFile.close();
    display.drawString(0,10,"Done!");
    display.display();
    
		// Only Write EOF of not writing to LCD
		if(bDelayOutLevel <= 1) Serial.write(EF);
		Serial.flush();

		return eError;

	}

	ErrorCodes FileOutXmodem(String sFileName){
		ErrorCodes eError = OK1;

		File OutFile;
		OutFile = SD.open(sFileName);

		// Make OLED displaySerial.println("Xmodem ready");
   display.clear();
   display.drawString(0,0,"Xmodem Ready.");
   display.display();
		if(!OutFile){
			eError = FF;
		} else {
			// Convert to sFileName to CPP style string
			// Make OLED displaySerial.println("Xmodem Sending");
     display.drawString(0,10,"Xmodem Sending.");
     display.display();
			//MyXmodem.sendFile(OutFile, sFileName.c_str());
      char buffer[15];
      sFileName.toCharArray(buffer, 15);
      MyXmodem.sendFile(OutFile, buffer);
      //MyXmodem.sendFile(OutFile, "testme.ba");
		}

		OutFile.close();
		return eError;
	}

	ErrorCodes FileInDelete(String sFileName, byte bDelayOutLevel = 0){
		ErrorCodes eError = OK1;
		if (SD.exists(sFileName)) {
			SD.remove(sFileName);
		} else {
			eError = FF;
		}
		// Assume for 'DEL' command we are writing to the LCD
		if(bDelayOutLevel > 0){
			Serial.write(CR);
			Serial.write(LF);
			delay(OUT_DELAY*(bDelayOutLevel*2));
		}
	}

	ErrorCodes FileInOpen(String sFileName){
		ErrorCodes eError = OK1;

		FileInDelete(sFileName);

		InFile = SD.open(sFileName, FILE_WRITE);

		if(!InFile){
			InFile.close();
			eError = FF;
		}

		return eError;
	}

	ErrorCodes FileDirOut(byte bDelayOutLevel = 4){

		ErrorCodes eError = OK1;

		File DirFile = SD.open("/");
		File FileInfo;

		if(!DirFile){
			eError = FF;
		} else {

			Serial.write(CR);
			Serial.write(LF);
			delay(OUT_DELAY*(bDelayOutLevel*4));

			//DirFile.rewindDirectory();

			while (true) {
				FileInfo = DirFile.openNextFile();

				if(!FileInfo){
					// no more files
					break;
				} else {
					//if (!FileInfo.isDirectory() && strlen(FileInfo.name()) < 10)  {

						// Only list files with a 2 character extension
						//if(FileInfo.name()[strlen(FileInfo.name())-3] == '.'){

							Serial.write(FileInfo.name());
              Serial.print("\t\t");
              Serial.println(FileInfo.size(), DEC);
							//Serial.write(FileInfo.name());
							delay(OUT_DELAY*bDelayOutLevel);

							/*for (byte iCount = strlen(FileInfo.name()) ; iCount < 10 ; iCount++){
								Serial.write(32);
								Serial.write(32);
							}*/

							delay(OUT_DELAY*bDelayOutLevel);

						//}

					//}
				}

				FileInfo.close();
			}

			Serial.write(CR);
			Serial.write(LF);
			delay(OUT_DELAY*bDelayOutLevel);

		}

		DirFile.close();
		//Serial.write(EF);
		Serial.flush();

		return eError;

	}


	// Return true if file is open
	boolean FileInWrite(char cInChar) {
  display.clear();
  display.drawString(0,0,"Saving....");
  display.display();
		if(cInChar != EF){
			Serial.print(cInChar);
			InFile.print(cInChar);
			return true;
		} else {
			InFile.close();
     display.drawString(0,10,"Done.");
     display.display();
			return false;
		}

	}


	public:

	// Open and read file
	// bBasicFile needs to be true if loading or saving a 'BASIC' file directly
	ErrorCodes ReadFile(String sFileName, byte bDelayOutLevel = 1){
		// Open SD card reader
		return FileOut(sFileName, bDelayOutLevel);
	}

	ErrorCodes ReadFilesDir(byte bDelayOutLevel = 8){
		// Open SD card reader
		return FileDirOut(bDelayOutLevel);
	}

	ErrorCodes FileXGet(String sFileName){
		return FileOutXmodem(sFileName);
	}

	ErrorCodes FileDelete(String sFileName,byte bDelayOutLevel = 1){
		// Open SD card reader
		return FileInDelete(sFileName, bDelayOutLevel);
	}

	ErrorCodes OpenFile(String sFileName){
		// Open SD card reader
		return FileInOpen(sFileName);
	}

	boolean OutFile(char cInChar){
		return FileInWrite(cInChar);
	}

	void OutFileTimeOut(){
    display.clear();
    display.drawString(0,0,"Timeout: File Closed");
    display.display();
		InFile.close();
	}

  void modemSetup() {
  
    //ESPhttpUpdate.rebootOnUpdate(false);
    EEPROM.begin(LAST_ADDRESS + 1);
    delay(10);
    if (EEPROM.read(VERSION_ADDRESS) != VERSIONA || EEPROM.read(VERSION_ADDRESS + 1) != VERSIONB) {
        defaultEEPROM();
    }
    readSettings();
    // Fetch baud rate from EEPROM
    serialspeed = EEPROM.read(BAUD_ADDRESS);
    // Check if it's out of bounds-- we have to be able to talk
    if (serialspeed < 0 || serialspeed > (sizeof(bauds)/sizeof(long))) {
        serialspeed = 0;
    }
    Serial.begin(bauds[serialspeed]);
    // OLED: Initialising the UI will init the display too.
    display.init();
    //display.flipScreenVertically();
    //display.setContrast(129);
    display.setFont(ArialMT_Plain_10);
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    //display.drawString(64, 0, "C64-WiFiModem v" + String(VERSIONA) + "." + String(VERSIONB) + "." + build);
    display.drawString(64, 0, firmwareVersion);
    display.drawString(64, 12, "by my2bits");
    display.drawString(64, 48, "Press <RETURN> to begin");
    display.display();
    // Wait loop until correct keypress sent to modem
    char c;
    //unsigned long startMillis = millis();
    //while (c != 8 && c != 127 && c!= 20) { // Check for the backspace key to begin
    //while (c != 32) { // Check for space to begin
    while (c != 0x0a && c != 0x0d) { // Check for RETURN to begin
        if (Serial.available() > 0) {
            c = Serial.read();
            if (petTranslate == true){
                if (c > 127) c-= 128;
            }
        }
        
        yield();
    }
    welcome();
    /*
    if (tcpServerPort > 0) tcpServer.begin();
    WiFi.mode(WIFI_STA);
    connectWiFi();
    sendResult(R1_OK);
    //tcpServer(tcpServerPort); // can't start tcpServer inside a function-- must live outside
    digitalWrite(LED_PIN, LOW); // on
    webServer.on("/", handleRoot);
    webServer.on("/ath", handleWebHangUp);
    webServer.begin();
    //mdns.begin("C64-WiFiModem", WiFi.localIP());
    MDNS.begin("C64-WiFiModem");
    */
    modemLoop();
}
};


// *************************************
// * User Command from Serial in / out *
// *************************************
class SerialInOut {

	private:

	ErrorCodes eLastError;

	SDCardDrive InOutSD;
	SerialStates eComState = FREE;
	String sInString;

	// Track Command In
	unsigned long iUpdateLast = 0;

	// Simple filename check: Filename valid on a T100
	// Filename must hav and extension
	// Filename should only be up to 6 CHAR with 2 CHAR extension with '.' separator
	// bBasicFile needs to be true if loading or saving a 'BASIC' file directly
	ErrorCodes CheckFileName(boolean bBasicFile = false){
		String sExtension;
		// Check filename length withn 6 CHAR
		if(sInString.indexOf('.') < 8 && sInString.indexOf('.') == sInString.lastIndexOf('.')){
			// Check filename extension is 2 CHAR
			if(sInString.length()-sInString.indexOf('.')-1 == 2){
				sExtension = sInString.substring(sInString.indexOf('.')+1,sInString.length());
				// if checking and LOADING a file to BASIC allow extension BA and DO
				if(bBasicFile == false || (bBasicFile == true && (sExtension == "BA" || sExtension == "DO"))){
					return OK1;
				}
			}
		}
		return NM;
	}

	ErrorCodes GetCommand(){

		ErrorCodes eError = OK1;

		
		sInString.toUpperCase();
    sInString.trim();

		// Cycle through KEYWORDS for matching command
		for (int bKeyword = 0 ; bKeyword < 15 ; bKeyword++){
			if(sInString.length() >= KEYWORDS[bKeyword].length()){
				if (sInString.startsWith(KEYWORDS[bKeyword])){
					// Trim Command from String
					sInString = sInString.substring(KEYWORDS[bKeyword].length());
					sInString.trim();
					// If Command found and we need an ARG such as a filename
					if (sInString.length() != 0 || KEYWORDS[bKeyword] == "DIR" || KEYWORDS[bKeyword] == "FILES" || KEYWORDS[bKeyword] == "HELP" || KEYWORDS[bKeyword] == "MODEM" || KEYWORDS[bKeyword] == "MDM" || KEYWORDS[bKeyword] == "WIFI"){

						// ********************************
						// * Select Output to M100 / M102 *
						// ********************************

						if (KEYWORDS[bKeyword] == "LOAD"){
							// Flag as loading direct to BASIC
							eError = CheckFileName(true);
							if(eError==OK1){
								eError = InOutSD.ReadFile(sInString);
							}

							break;
						}

						// Send direct to a RAM file or LCD screen
						else if (KEYWORDS[bKeyword] == "COPY2R" || KEYWORDS[bKeyword] == "CAT"){
							
                 char cRead, cLast;
                 File MyFile = SD.open(sInString);
                 display.clear();
                 display.drawString(0,0,"Load/CAT...");
                 display.drawString(58,0,sInString);
                 display.display();
                 if (MyFile) {
                  while(MyFile.available()){
                    cRead=MyFile.read();
                    if (cRead == LF) {
                      if (cLast != CR); {
                      Serial.write(CR);
                      
                    }
                    Serial.write(LF);
                  } else {
                    Serial.write(cRead);
                    cLast=cRead;
                  }
                 }
                 }
                 MyFile.close();
                 display.drawString(0,10,"Done!");
                 display.display();
                 Serial.write(EF);
								break;
						}

						else if (KEYWORDS[bKeyword] == "XGET"){
							eError = CheckFileName(false);
							if(eError==OK1){
								eError = InOutSD.FileXGet(sInString);
							}
							break;
						}


						else if (KEYWORDS[bKeyword] == "DIR" || KEYWORDS[bKeyword] == "FILES"){

							eError = InOutSD.ReadFilesDir();
							break;
						}

						else if (KEYWORDS[bKeyword] == "DEL" || KEYWORDS[bKeyword] == "KILL"){
							eError = CheckFileName(false);
							if(eError==OK1){
								eError = InOutSD.FileDelete(sInString,2);
							}
							break;
						}

						// *********************************
						// * Select Input from M100 / M102 *
						// *********************************

						else if (KEYWORDS[bKeyword] == "SAVE") {
							// Check Filename
							eError = CheckFileName(true);
							if(eError==OK1) eError = InOutSD.OpenFile(sInString);
							// Once SD card is opened
							if(eError==OK1) {
								eComState = DATA_IN;
							}

							break;
						}

						else if (KEYWORDS[bKeyword] == "COPY2D") {
							// Check Filename
							eError = CheckFileName(false);
							if(eError==OK1) eError = InOutSD.OpenFile(sInString);
							// Once SD card is opened
							if(eError==OK1) {
								eComState = DATA_IN;
							}

							break;
						}

           else if (KEYWORDS[bKeyword] == "HELP") {
            //Serial.println("LOAD,SAVE,DEL,DIR,COPY2R,COPY2D,CAT,XGET,FILES,KILL,HELP");
            display.clear();
            display.drawString(0,0,"LOAD, SAVE, DEL, DIR,");
            display.drawString(0,9,"COPY2R, COPY2D, CAT,");
            display.drawString(0,18,"XGET, FILES, KILL");
            display.display();
            eComState = FREE;
            break;
           }
           else if (KEYWORDS[bKeyword] == "MODEM" || KEYWORDS[bKeyword] == "WIFI" || KEYWORDS[bKeyword] == "MDM") {
            eComState = MODEM;
            break;
           }

						//string.startsWith(string2)
					}
				}
			}
		}

		if(eComState != DATA_IN) eComState = FREE;
		sInString = "";
		return eError;
	}

	public:

	void NoDataIn(){

		unsigned long iUpdateTime = millis();

		// Check last DATA_IN Command hasn't timed out, if so close it
		if (iUpdateTime - iUpdateLast >= TIMEOUT && eComState == DATA_IN) {
			InOutSD.OutFileTimeOut();
			eComState = FREE;
		}

	}

	void SerialIn(char cInChar){

		boolean WriteFileOpen;

		iUpdateLast = millis();

		switch(eComState) {
			case FREE:
				if(cInChar == CR){
					// Set to send data from T100 on Command in
					eComState = DATA_OUT;
					eLastError = GetCommand();
				} else {
					sInString += cInChar;
				}
				break;
			case DATA_IN:
					WriteFileOpen = InOutSD.OutFile(cInChar);
					if (WriteFileOpen == false) eComState = FREE;
				break;
		  case MODEM:
        modemSetup();
      break;
	  }
	}
};

// **** Global Variables  / Classes ****
SerialInOut InOut;


void setup(){
	Serial.begin(BAUD_RATE);


display.init();
display.setFont(ArialMT_Plain_10);
 display.drawString(19, 0, "WiFi SD Card Drive");
display.drawString(13,9,"By LeRoy Miller, 2019");
display.drawString(0,19,"Based on: MyTeSed By");
//display.drawString(0,29,"MyTeSed By");
display.drawString(0,29,"David Stephenson, 2017");
display.drawString(0,39,"& C64-WiFiModem by");
display.drawString(0,49,"my2bits & others, 2017");
display.display();
delay(5000);
display.clear();
	if(!SD.begin(5)){
  //OLED Display Card Ready/Not Ready
		//Serial.println("SD Card Not Found");
    display.drawString(0,0,"SD Card Not Found.");
    display.drawString(0,50,"Power OFF before inserting card.");
    display.display();
    while(1);
	} else {
		//Serial.println("SD Card Found");
    display.drawString(0,0,"Ready.");
    display.display();
	}
 
}


void loop()
{
	char cInChar;

	while (Serial.available()) {
		cInChar = (char)Serial.read();
		InOut.SerialIn(cInChar);
	}

	InOut.NoDataIn();

}
