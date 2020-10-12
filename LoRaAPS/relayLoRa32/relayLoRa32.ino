//This sketch is written for the LoRa32 ESP32 board, the oled is not used, It should be used as a RELAY/WIDE digipeater
//It can also be used as a serial interface if connected to a computer. But would take some work to get it to gate to the MQTT broker
//This sketch should be able to be adapted for other AVR boards 
// September 2020, base version code 0.0.5
// September 16, 2020 version 0.0.7 base removed "F" from transmit, changed checks to check "P",0 for callsign (This should save a little more space in the json string) 

/* Copyright (c) 2020 LeRoy Miller, KD8BXP
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

 */
  
#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include <ArduinoJson.h> //version 5.13.5 (needs updated)
#include <TimedAction.h> //used to ID the digipeater ever 10 minutes. 
// Original Library by Alexander Brevig https://playground.arduino.cc/Code/TimedAction/
// A fork of the project was found on Github https://github.com/Glumgad/TimedAction

//These must match for your specific board, these should work for the LoRa32u4 boards, but if it fails, check here.
#define SS      18    
#define RST     14
#define DI0     26
#define BAND    432300000 // local frequencys, must match!

#define LED 13 //LED Pin

#define CQMSG "LoRaAPS net digipeater"
String CALLSIGN="KD8BXP-01"; //this will be appended to the message when a packet is digipeated. This is also the callsign to Beacon an ID 

//For this to work on a local level these parameters need to match
int       loraSpreadingFactor = 9;
int       loraCodingRate      = 5;
int       loraTxPower         = 17;
uint32_t  loraBandwidth       = 62500; //125E6;
//This results in about 879bps if you believe this site https://unsigned.io/understanding-lora-parameters/
//or maybe 488bps from this website https://www.rfwireless-world.com/calculators/LoRa-Data-Rate-Calculator.html

String inputString;
bool stringComplete = false;
String call, msg;
String radiopacket;
bool callPassCheck = false;
bool msgPassCheck = false;
bool pass1 = false;
bool pass2 = false;
bool pass3 = false;

void beacon();
TimedAction beaconAction 	= 	TimedAction(600000,beacon); //send an ID about every 10 minutes

void setup() 
{

  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  Serial.begin(9600);
  Serial.setTimeout(10);
  //while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  inputString.reserve(200);
  SPI.begin(5,19,27,18);
  radioon();

}

void loop(){
  
  serialEvent(); //The Leonardo (ATMEGA32u4 doesn't support serialEvent(), so we can just call it from the loop)
  rx();
  beaconAction.check(); //check if it is time to send a beacon (ID the digipeater)
    
}
