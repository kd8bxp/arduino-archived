//This sketch is written for the TTGO T-Beam w/OLED v1 board, It should be used as a PAGER and digipeater
//Bluetooth enabled for using a Serial Bluetooth terminal on the phone.  
//It can also be used as a serial interface if connected to a computer. 
//This sketch should be able to be adapted for other boards (AVR/ESP32)
// September 2020, base version code 0.0.5
// Sept 14, 2020 added serial/bluetooth print messages v0.0.6
// September 16, 2020 version 0.0.7 base removed "F" from transmit, changed checks to check "P",0 for callsign (This should save a little more space in the json string)
// September 16, 2020 version 0.0.7 (a) added voltage reading for display (only for T-Beam)
// Sept 19, 2020 version 0.0.8 added button, added display path for T-Beam, and TTGO LoRa32
// Sept 22, 2020 v 0.0.8(a) corrected path display error
// Oct 15, 2020 v 0.0.9 added GPS information to display

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
#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino
#include <Wire.h>
#include "SSD1306Ascii.h" //https://github.com/greiman/SSD1306Ascii
#include "SSD1306AsciiWire.h"
#include <TinyGPS++.h> //https://github.com/mikalhart/TinyGPSPlus
#include <axp20x.h> //https://github.com/lewisxhe/AXP202X_Library
#include "Button2.h"; //https://github.com/LennartHennigs/Button2

//These must match for your specific board, these should work for the LoRa32u4 boards, but if it fails, check here.
#define SS      18    
#define RST     14
#define DI0     26
#define BAND    432300000 // local frequencys, must match!

#define LED 2 //LED Pin

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define BUTTON_PIN  38 //Button for TTGO T-Beam v1

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//#define OLED_RESET     16 // Reset pin # (or -1 if sharing Arduino reset pin)
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
//#define RST_PIN 16
SSD1306AsciiWire oled;

#define CQMSG "LoRaAPS net pager"
String CALLSIGN="KD8BXP-03"; //this will be appended to the message when a packet is digipeated. This is also the callsign to Beacon an ID 

//For this to work on a local level these parameters need to match
int       loraSpreadingFactor = 9;
int       loraCodingRate      = 5;
int       loraTxPower         = 17;
uint32_t  loraBandwidth       = 62500; //125E6;
//This results in about 879bps if you believe this site https://unsigned.io/understanding-lora-parameters/
//or maybe 488bps from this website https://www.rfwireless-world.com/calculators/LoRa-Data-Rate-Calculator.html

String inputString, msg;
bool stringComplete = false;
String call;
String radiopacket;
bool callPassCheck = false;
bool msgPassCheck = false;
bool pass1 = false;
bool pass2 = false;
bool pass3 = false;
String baChStatus = "No charging"; //battery status
int displayChange = 1;
String from = "N0CAL-00";
String holdMsg = "No Message";
String path[4] = {"","NOCALL1", "NOCALL2", "NOCALL3"};
String rssi;

void beacon();
TimedAction beaconAction 	= 	TimedAction(600000,beacon); //send an ID about every 10 minutes
BluetoothSerial ESP_BT; //Object for Bluetooth
TinyGPSPlus gps;
HardwareSerial GPS(1);
AXP20X_Class axp;
Button2 button = Button2(BUTTON_PIN);

void setup() 
{

  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  Serial.begin(9600);
  Serial.setTimeout(10);
  ESP_BT.begin(CALLSIGN); //Using the callsign to be easier to find the device to use
  //while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  inputString.reserve(200);
  SPI.begin(5,19,27,18); //TTGO SPI PINs
  Wire.begin(21,22); //TTGO T-BEAM OLED
  setupButtons(); 
  startGPS();
  displaysetup();
  radioon();
  batteryCheck(); //Display callsign of device, and battery voltage for the TTGO T-Beam
}

void loop(){
  
  serialEvent(); //The Leonardo (ATMEGA32u4 doesn't support serialEvent(), so we can just call it from the loop)
  BTEvent();
  rx();
  beaconAction.check(); //check if it is time to send a beacon (ID the digipeater)
  getGPS();
 // batteryCheck();
   button.loop();
}
