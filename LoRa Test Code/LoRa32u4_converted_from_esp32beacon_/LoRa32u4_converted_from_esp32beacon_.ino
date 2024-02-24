/* This is a beacon for the LoRaHam protocol by KK4VCZ.
 * https://github.com/travisgoodspeed/loraham/
 * 
 * Modified by LeRoy Miller, KD8BXP
 * Aug 25, 2017 For use with the TTGO WIFI_LoRa_32 Boards
 * https://www.aliexpress.com/item/TTGO-loraSX1278-ESP32-0-96OLED-16-Mt-bytes-128-Mt-bit-433Mhz-for-arduino/32824758955.html
 * 
 * Significant changes:
 * 
 * Library Changed to: arduino-LoRa
 * https://github.com/sandeepmistry/arduino-LoRa
 * 
 * Attempt to Tokenized LoRaHam Protocol to use JSON
 * Library: https://github.com/bblanchon/ArduinoJson/
 * 
 */

 /*
  * Converted from the ESP32 Beacon sketch, which used json to send messages, which resluts in invalid pkt recieved on a reciever end
  * Aug 29, 2020, LeRoy Miller
  */
 
#define TOCALL "KD8BXP"
#define CALLSIGN "KD8BXP-10" 
#define COMMENTS "2600mAh ESP32" 
 
#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
//#include <Wire.h>
//#include "SSD1306.h"
#include <ArduinoJson.h>

//#define OLED_RESET  16  // Pin 16 -RESET digital signal
//#define OLED_SDA    4  // SDA-PIN for I2C OLED
//#define OLED_SCL    15  // SCL-PIN for I2C OLED

//SSD1306 display(0x3c, OLED_SDA, OLED_SCL); // FOR I2C

// WIFI_LoRa_32 ports

// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      8
#define RST     4
#define DI0     7
#define BAND    432300000 // Change to 434.0 or other frequency, must match RX's freq!

#define VBATPIN A5 //battery voltage check pin
#define LED 13 //LED Pin

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300        /* Time ESP32 will go to sleep (in seconds) 5 minutes*/

//RTC_DATA_ATTR int packetnum=0;
int packetnum=0;
     int       loraSpreadingFactor = 9;
  int       loraCodingRate      = 5;
  int       loraTxPower         = 17;
  uint32_t  loraBandwidth       = 62500; //125E6;

void setup() 
{
 // pinMode(OLED_RESET,OUTPUT);
 // digitalWrite(OLED_RESET, LOW);    // set GPIO16 low to reset OLED
 // delay(50); 
 // digitalWrite(OLED_RESET, HIGH); // while OLED is running, must set
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(10);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);

 // display.init();
 // display.flipScreenVertically();
 // display.setFont(ArialMT_Plain_10);
 // display.clear();
 /* display.setColor(WHITE);
  display.drawString(0,0,"Callsign: ");
  display.drawString(45, 0,CALLSIGN);
  display.drawString(35,11,"LoRaHam Beacon");
  display.drawString(47,22,"by KK4VCZ.");
  display.drawString(32,33,"modified for ESP32");
  display.drawString(50,44,"by KD8BXP.");
  display.display();
  delay(3000); //while delayed can't get messages
  display.clear();
  display.display();
*/

  //SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc(); //encable CRC checking - off by default
    LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
  digitalWrite(LED, LOW);
  //esp_deep_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

//added for Json

}

void loop(){
  //Transmit a beacon once every five minutes.
  beacon();
  //Then turn the radio off to save power.
  radiooff();
  //esp_deep_sleep_start();
}
