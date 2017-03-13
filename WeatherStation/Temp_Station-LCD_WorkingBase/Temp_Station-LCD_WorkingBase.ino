// TFM WeatherStation
// version 0.2
// TFM - Peter Hein 
//
// LCD pint outs LCD 5110
// 1  VCC - Supply range is between 2.7V and 3.3V
// 2 GND - Ground    
// 3 SCE - Screen
// 4 RST - Reset 
// 5 D/C - Mode
// 6 DN(MOSI)
// 7 SCLK  
// 8 LED
//
// LCD5110 myGLCD(8,9,10,11,12)
//


#include <LCD5110_Basic.h>
#include <Wire.h>
#include "DHT.h"

//Define the DHT22 sensor
#define DHTPIN 2
#define DHTTYPE DHT22 

//The Nokia 5110
 LCD5110 myGLCD(8,9,10,11,12); 
    extern uint8_t SmallFont[]; 
    extern uint8_t MediumNumbers[]; 
    extern uint8_t BigNumbers[];

// Setup DHT
DHT dht(DHTPIN, DHTTYPE);    

void setup() {
  // Setup the LCD
  myGLCD.InitLCD(); 
  myGLCD.setContrast(60);
  
  Serial.begin(9600);
 
  dht.begin();
}

void loop() {
  // Check the temperature and humidity
float hum = dht.readHumidity();
float temp = dht.readTemperature();
delay(3000);

  // Check to see if the DHT is present
  // if not print Not Found
  if (isnan(hum) || isnan(temp)) {
    myGLCD.clrScr();
   myGLCD.setFont(SmallFont);
   myGLCD.print("Device Not Found",15,1);
   Serial.print(temp);
   myGLCD.printNumI(temp,30,20);
    delay(2000);
    
  }
else {
  // Time to print some data
  // Need to work on the formatting. 
  // Goal: to put all the reading on one screen
  // Goal: Historical data on graph
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("TFM Weather Station",2,1);
  delay(2000);
  myGLCD.clrScr();
  myGLCD.print("How is the we",4,1);
  myGLCD.print("ather?",5,1);
  delay(2000);
  // testing placement of text on the screen
  // remove later
    //Print UI
    myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("TEMP",LEFT+10,0);
  myGLCD.print("HUM",LEFT+58,0);
delay(3000);
  // REMOVE TO HERE
myGLCD.clrScr();
myGLCD.setFont(SmallFont);
myGLCD.print("Temperature:",2,1);

myGLCD.setFont(MediumNumbers);
Serial.print(temp);
myGLCD.printNumI(temp,30,20);
delay(6000);

myGLCD.clrScr();
myGLCD.setFont(SmallFont);
myGLCD.print("% Humidity:",2,1);


myGLCD.setFont(MediumNumbers);
myGLCD.printNumI(hum,30,20);
delay(6000); 
} 
}
