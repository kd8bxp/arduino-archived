/*
  ATmega128RFA1 Dev Board Basic Chat
  by: Jim Lindblom
      SparkFun Electronics
  date: July 3, 2012
  License: Beerware. Feel free to use, reuse, and modify this code
  as you please. If you find it useful, you can buy me a beer.

  This code sets up the ATmega128RFA1's wireless transciever in
  the most basic way possible to serve as a serial gateway.
  Serial into the ATmega128RFA1's UART0 will go out the RF radio.
  Data into the RF radio will go out the MCU's UART0.

  Modified by LeRoy Miller for the Smart Response XE Mar 19, 2021
  
*/

#include "RadioFunctions.h"
#include "SmartResponseXE.h"

void setup()
{
  //Serial.begin(9600);  // Start up serial
  //Serial1.begin(115200);
  SRXEInit(0xe7, 0xd6, 0xa2); // Initialize the LCD
  rfBegin(11);  // Initialize ATmega128RFA1 radio on channel 11 (can be 11-26)
  
  // Send a message to other RF boards on this channel
  rfPrint("ATmega128RFA1 Dev Board Online!\r\n");
  SRXEWriteString(0,96,"Online!",FONT_MEDIUM,3,0);
}

void loop()
{
  
  //if (Serial.available())  // If serial comes in...
  //{
    rfWrite(SRXEGetKey()); //Serial.read()); // ...send it out the radio.
  //}
  if (rfAvailable())  // If data receievd on radio...
  {
    char temp = rfRead();
    SRXEWriteString(0,0,&temp,FONT_MEDIUM,3,0);  // ... send it out serial.
  }
}
