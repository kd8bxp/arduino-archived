/* LoRaHam Pager Example
 * 
 * Pager Sketch by KK4VCZ.
 * 
 * Modified by LeRoy Miller, KD8BXP
 * Aug 25, 2017 For use with the TTGO WIFI_LoRa_32 Boards
 * https://www.aliexpress.com/item/TTGO-loraSX1278-ESP32-0-96OLED-16-Mt-bytes-128-Mt-bit-433Mhz-for-arduino/32824758955.html
 * 
 * Significant changes:
 * LoRa Library Changed to: arduino-LoRa
 * https://github.com/sandeepmistry/arduino-LoRa
 * 
 * Addfruit_SSD1306 library changed to: squix78 esp8266 ssd1306 library
 * https://github.com/squix78/esp8266-oled-ssd1306
 * 
 * Attempt to Tokenized LoRaHam Protocol to use JSON
 * Library: https://github.com/bblanchon/ArduinoJson/
 * 
 * Sept 2, 2017 Added ESP32 Webserver 
 * Library Needed: http://www.fisch.lu/junk/ESP32-WebServer.zip (unofficial port of the ESP8266WebServer Library)
 * More Information can be found in the Arduino-ESP32 issue tracker on github
 * https://github.com/espressif/arduino-esp32/issues/425
 * 
 * Webserver default address (192.168.4.1) At this time I'm not sure how to change it.
 * The webserver is made for small screens (Phones/Tablets) but does work from a desktop.
 * root directory will show last message.
 * /msg allows you to send a message (not yet fully ready for use)
 * /msg2 is the API call 192.168.4.1/msg2?TO=KD8BXP&MSG=Some Message
 * /cfg setup and change configuration of your pager from a website (not yet ready)
 * 
 * Sept 9, 2017 Broke out functions to own tabs (easier to read, easier to edit),
 * added code to RT (digipeat) and (Sept 10) added send message code from website
 * 
 *  *Breaking the functions out will also help to add or create a new device.
 *      * digipeat and sendMSG are almost the same code, it should be possiable to combine with some if statements (etc)
 *      
 * Wish List/Ideas to improve:
 * 
 * 1) Add buzzer, Buzzer when a message arrives for your Callsign
 *    a different sound for BEACON messages (*DONE*)
 * 2) Add dip switches to turn on/off features.    
 *    one feature: ALL messages (BEACON) or CALLSIGN ONLY messages (in Webserver)
 * 3) WIFI website or api to send messages (in progress)   
 * 4) MAYBE a bluetooth keyboard to send messages (rejected)
 * 5) add button to review previous received message. (*In webserver - Works DONE*)
 * 
 */
 
String CALLSIGN = ""; //USER Defined Special CallSign to receive and display
String MYCALL = "KD8BXP"; //display messages addressed to my call (or my call and ssid)
//May need to add a append SSID to MYCALL option 
String MYDEVICETYPE = "PAGER"; //can be GATEWAY, BEACON, DIGIPEAT, or PAGER
String MYGRID = "EM79";
String MYDEVICEID; //figure what I need to do to make a device ID 
String RXTIME, RXDATE, CFGOTHER;

#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include <Wire.h>
#include "SSD1306.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESP32WebServer.h>

#define OLED_RESET  16  // Pin 16 -RESET digital signal
#define OLED_SDA    4  // SDA-PIN for I2C OLED
#define OLED_SCL    15  // SCL-PIN for I2C OLED

SSD1306 display(0x3c, OLED_SDA, OLED_SCL); // FOR I2C
ESP32WebServer server(80);

// WIFI_LoRa_32 ports

// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6 // Change to 434.0 or other frequency, must match RX's freq!

#define CHANNEL 0 //Used for Tone Generation
#define TONEPIN 32 //Pin USED for buzzer

// Blinky on receipt
#define LED 25

String TO, FROM, MSG1, TXTIME, TXDATE, GRID, TYPE, OTHER, TXDEVICEID;
char RTCALL[5],RTRSSI[5],RTDEVICEID[5],RTDEVICETYPE[5],RTTIME[5],RTDATE[5];
int RTCNT;
char ssid[15]; //Create a Unique AP from MAC address * Creates an AP named "LoRaHam-xxxx' where xxxx is part of the MAC address of device.
const char *password = "pass1234"; //Access Point (AP) Password: not a great password, you may want to change it. 
static int serverCore = 0; //run web server on this core. Loop() runs in core 1
int rssi=0;

void setup() {
  createSSID();
  pinMode(OLED_RESET,OUTPUT);
  digitalWrite(OLED_RESET, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(OLED_RESET, HIGH); // while OLED is running, must set GPIO16 in high
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

ledcSetup(CHANNEL, 2000, 8);
ledcAttachPin(TONEPIN, CHANNEL);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  displayIntro();
  
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc(); //encable CRC checking - off by default
  digitalWrite(LED, LOW);
   WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.print(myIP);
    server.on("/", webRoot);
    server.on("/cfg", webCFG);
    server.on("/msg", webEnterMSG);
    server.on("/msg2", webProcessMSG);
    server.begin();

    xTaskCreatePinnedToCore(
                    serverTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    serverCore);  /* Core where the task should run */
 
}

void loop() {
  // put your main code here, to run repeatedly:
  pager();
  vTaskDelay(10);
}
