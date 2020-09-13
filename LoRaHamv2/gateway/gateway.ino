//This sketch is written for the LoRa32u4 board, It should be used as a RELAY/WIDE digipeater
//It can also be used as a serial interface if connected to a computer. But would take some work to get it to gate to the MQTT broker
//This sketch should be able to be adapted for other AVR boards 
 
#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include <ArduinoJson.h> //version 5.13.5 (needs updated)
#include <TimedAction.h> //used to ID the digipeater ever 10 minutes. 
// Original Library by Alexander Brevig https://playground.arduino.cc/Code/TimedAction/
// A fork of the project was found on Github https://github.com/Glumgad/TimedAction

//These must match for your specific board, these should work for the LoRa32u4 boards, but if it fails, check here.
#define SS      8    
#define RST     4
#define DI0     7
#define BAND    432300000 // local frequencys, must match!

#define LED 13 //LED Pin

#define CQMSG "LoRaHam net v2.0 digipeater"
String CALLSIGN="KD8BXP-10"; //this will be appended to the message when a packet is digipeated. This is also the callsign to Beacon an ID 

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

void cq();
TimedAction asciiTableAction 	= 	TimedAction(600000,cq); //send an ID about every 10 minutes

void setup() 
{

  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  Serial.begin(9600);
  Serial.setTimeout(10);
  //while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  inputString.reserve(200);
  
  radioon();

}

void loop(){
  
  serialEvent(); //The Leonardo (ATMEGA32u4 doesn't support serialEvent(), so we can just call it from the loop)
  rx();
  cq.check(); //check if it is time to send a beacon (ID the digipeater)
  //radiooff();
  
}
