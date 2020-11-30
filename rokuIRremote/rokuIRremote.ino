// 9/25/17  last update  **developmental**


#include "src/IRremote/IRremote.h"

// ROKU IR
unsigned int RokuHome[] = {1,67,8950,4500,600,550,550,1700,550,600,550,1700,550,550,600,1650,600,1650,600,1650,600,550,550,1700,550,600,550,550,600,550,550,600,550,1700,550,1700,550,1700,550,1700,550,550,600,550,550,600,550,550,600,550,550,600,550,550,600,550,550,1700,550,1700,600,1650,550,1700,550,1700,550,1700,600}; //5743C03F

int khz = 38;

//IRrecv irrecv(9);
decode_results results;
IRsend irsend;

void setup()
{
Serial.begin(9600);
  Serial.println("Enabling IRin");
  //irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
 
 //if (irrecv.decode (&results)) {
 //   Serial.println(results.value, HEX);
 //   irrecv.enableIRIn();
   
/****************************ROKU*********************************************ROKU*************************/   
int code = (0x20250AF);

switch (code) {

  case 0x20250AF:                                  // watch Roku
        Serial.println("Watch Roku Received");
        irsend.sendRaw(RokuHome, sizeof(RokuHome) / sizeof (RokuHome[0]), khz);
        Serial.println("...TV On");
        delay(1000);
        Serial.println("...Roku Phone Home");
        delay(1000);
        Serial.println("... Setting TV Input"); //TV's CEC setting ON. 'RokuHome' turns TV on, and sets HDMI2 automatically
        delay(1000);
        Serial.println("Watching Roku!");
        delay(1);
        //irrecv.enableIRIn();
        break;     
   
  /*case 0x20258A7:                                   // roku home
        Serial.println("Home Received");
        irsend.sendRaw(RokuHome, sizeof(RokuHome) / sizeof (RokuHome[0]), khz);
        delay(100);
        Serial.println("Phoned Home");
        delay(1);
        //irrecv.enableIRIn();
        break;
 
  case 0x20202FD:                                 // roku back
        Serial.println("Back Received");
        irsend.sendRaw(RokuBack, sizeof(RokuBack) / sizeof (RokuBack[0]), khz);
        delay(100);
        Serial.println("Back Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;
 
  case 0x202D02F:                              // roku up
        Serial.println("Up Received");
        irsend.sendRaw(RokuUp, sizeof(RokuUp) / sizeof (RokuUp[0]), khz);
        delay(100);
        Serial.println("Up Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;
 
  case 0x202708F:                                  // roku down
        Serial.println("Down Received");
        irsend.sendRaw(RokuDown, sizeof(RokuDown) / sizeof (RokuDown[0]), khz);
        delay(100);
        Serial.println("Down Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;
 
  case 0x20208F7:                                  // roku left
        Serial.println("Left Received");
        irsend.sendRaw(RokuLeft, sizeof(RokuLeft) / sizeof (RokuLeft[0]), khz);
        delay(100);
        Serial.println("Left sent");
        delay(1);
        //irrecv.enableIRIn();
        break;
 
  case 0x2028877:                                   // roku right
        Serial.println("Right Received");
        irsend.sendRaw(RokuRight, sizeof(RokuRight) / sizeof (RokuRight[0]), khz);
        delay(100);
        Serial.println("Right Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;
 
  case 0x202B04F:                             // roku OK
        Serial.println("OK Received");
        irsend.sendRaw(RokuOk, sizeof(RokuOk) / sizeof (RokuOk[0]), khz);
        delay(100);
        Serial.println("OK Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0xE1447A85:                                          // roku play/pause
        Serial.println("Play/Pause Received");
        irsend.sendRaw(RokuPlayPause, sizeof(RokuPlayPause) / sizeof (RokuPlayPause[0]), khz);
        delay(100);
        Serial.println("Play/Pause Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0xE1444AB5:                            // roku RW
        Serial.println("RW Received");
        irsend.sendRaw(RokuRW, sizeof(RokuRW) / sizeof (RokuRW[0]), khz);
        delay(100);
        Serial.println("RW Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0xE144CA35:                            // roku FF
        Serial.println("FF Received");
        irsend.sendRaw(RokuFF, sizeof(RokuFF) / sizeof (RokuFF[0]), khz);
        delay(100);
        Serial.println("FF Sent");
        delay(1);
        //irrecv.enableIRIn();
        break; 
 */

/****************************OPTIMUM*****************************************OPTIMUM*********************/   
 /*
  case 0x6F9C03F:                                 // Watch IO
        Serial.println("Watch Cable Received");
        irsend.sendRaw(TVPWR, sizeof(TVPWR) / sizeof (TVPWR[0]), khz);
        delay(1000);
        Serial.println("...TV On");
        irsend.sendRaw(CBL0, sizeof(CBL0) / sizeof (CBL0[0]), khz);
        Serial.println("...CableVision");
        delay(1000);
        Serial.println("... Setting TV Input");
        delay(6500);
        irsend.sendRaw(TVHDMITOG, sizeof(TVHDMITOG) / sizeof (TVHDMITOG[0]), khz);
        delay(1000);
        Serial.println("Watching Lies");
        delay(1);
        //irrecv.enableIRIn();
        break;     

  case 0x6F9F807:                              // IO Settings
        Serial.println("Settings Received");
        irsend.sendRaw(CBLSET, sizeof(CBLSET) / sizeof (CBLSET[0]), khz);
        delay(100);
        Serial.println("Settings Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0x6F938C7:              // IO Up
        Serial.println("Up Received");
        irsend.sendRaw(CBLUP, sizeof(CBLUP) / sizeof (CBLUP[0]), khz);
        delay(100);
        Serial.println("Up Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0x6F99867:              // IO Down
        Serial.println("Down Received");
        irsend.sendRaw(CBLDN, sizeof(CBLDN) / sizeof (CBLDN[0]), khz);
        delay(100);
        Serial.println("Down Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;
 */
  /**************************TV Control*******************************TV Control********************/
/*
  case 0xE144827D:                            // TV All Off
        Serial.println("All Off Received");
        irsend.sendRaw(TVAVIN, sizeof(TVAVIN) / sizeof (TVAVIN[0]), khz);
        delay(100);
        irsend.sendRaw(CBLPWRTOG, sizeof(CBLPWRTOG) / sizeof (CBLPWRTOG[0]), khz);
        delay(1000);
        irsend.sendRaw(TVPWR, sizeof(TVPWR) / sizeof (TVPWR[0]), khz);
        Serial.println("All Off Complete");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0x6F930CF:              // TV Vol Up
        Serial.println("Vol Up Received");
        irsend.sendRaw(TVVOLUP, sizeof(TVVOLUP) / sizeof (TVVOLUP[0]), khz);
        delay(100);
        Serial.println("Vol Up Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0x6F9B04F:              // TV Vol Down
        Serial.println("Vol Down Received");
        irsend.sendRaw(TVVOLUP, sizeof(TVVOLUP) / sizeof (TVVOLUP[0]), khz);
        delay(100);
        Serial.println("Vol Down Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0x6F9E01F:              // TV Mute
        Serial.println("Mute Received");
        irsend.sendRaw(TVMUTE, sizeof(TVMUTE) / sizeof (TVMUTE[0]), khz);
        delay(100);
        Serial.println("Mute Sent");
        delay(1);
        //irrecv.enableIRIn();
        break;

  case 0xE144BE41:             // TV Input Toggle
        Serial.println("Input Received");
        irsend.sendRaw(TVINPUT, sizeof(TVINPUT) / sizeof (TVINPUT[0]), khz);
        delay(100);
        Serial.println("Toggled Input");
        delay(1);
        //irrecv.enableIRIn();
        break;
   */
  }
while(1);
 }

//}
