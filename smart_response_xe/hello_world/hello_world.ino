#include "SmartResponseXE.h"

void setup() {
  // put your setup code here, to run once:
SRXEInit(0xe7, 0xd6, 0xa2); // Initialize the LCD
//PrepGUI(); // Draw our empty status text
//  rfBegin(26); // default to 802.15.4 channel 26 to upload code to the custom bootloader
  delay(3000); // wait a few seconds since the same serial port is used to update this device
  SRXEWriteString(0,96,"Hello World",FONT_MEDIUM,3,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
