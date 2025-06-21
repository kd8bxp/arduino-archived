//Anschliessen der DOT Matix
//DOT Matrix:       ESP8266 NodeMCU:
//VCC               5V (VUSB)
//GND               GND
//DIN               D7
//CS                D8
//CLK               D5

/*
 * Jun, 2019 - LeRoy Miller
 * https://github.com/kd8bxp/MQTT-D1-Mini-LED-Matrix-display
 * 
 *  The original sketch was found here:
 *  Smarte Display Box by Notagenius
 *  https://www.thingiverse.com/thing:3369864/files
 *  Which uses a single LED Matrix board, and a ESP8266 (D1 Mini)
 *  I was looking for a box that would hold the D1 Mini, and the Wemos
 *  LED Matrix shield, which is a small 8x8 matrix that sits on top of
 *  the D1 Mini.  The Box didn't work for my needs, but I found the sketch
 *  would.
 *  
 *  I modified it to use a 32x8 LED Matrix, using google translate
 *  translated it to mostly English, Added the WiFiManager so that it
 *  is easy to move to different locations.
 *  Added Over the Air updates, and a configuration file saved in SPIFFS
 *  also added a smile, and sad face, and change how the image is displayed 
 *  slightly, also added a NTP Clock. 
 *  Most of these are from the examples in provided by the ESP8266 board core.
 *  
 *  I found this case that works for the 32x8 Matrix and D1 Mini
 *  https://www.thingiverse.com/thing:2867294
 *  Mine printed a little small, and I needed to sand the LED Matrix to get it to
 *  fit, but it's a tight fit, and shouldn't cause a problem.
 *  
 *  Information on setting up SPIFFS can be found here:
 *  https://www.youtube.com/watch?v=jIOTzaeh7fs
 *  https://github.com/esp8266/arduino-esp8266fs-plugin/releases
 *  
 *  The only line that needs changed in the sketch is for the TIMEOFFSET
 *  currently set for Eastern time zone, United State.
 *  Setup your config.json file, upload using the information found in the 
 *  SPIFFS tutorial above.
 *  Then upload the sketch to your D1 Mini with a flash size of 4m(3m spiffs)
 *  which should be more than needed.
 *  Once it's uploaded, you should be able to upload a new config.json and 
 *  new firmware over the air as/if needed.
 *  
 */  
 
 
#include <ESP8266WiFi.h>
#include "src/PubSubClient/PubSubClient.h"
#include <SPI.h>             
#include "src/Adafruit_GFX/Adafruit_GFX.h"
#include "src/Max72xxPanel/Max72xxPanel.h"
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include "src/WiFiManager/WiFiManager.h"         //https://github.com/tzapu/WiFiManager
#include "src/ArduinoJson5/ArduinoJson.h"
#include "FS.h"

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

char mqtt_server[100];
String temp1;

#define TIMEOFFSET -14400 //-18000 //-14400 //Find your Time Zone off set Here https://www.epochconverter.com/timezones OFF Set in Seconds

bool AMPM = 1; //1 = AM PM time, 0 = MILITARY/24 HR Time
int hours,minutes, seconds;
unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

bool newmsg = false;
bool istext = false;
bool isimage = false;
bool isboth = false;
String mqttname;
char mqttnametemp[50];
char host[50];
String statustop;
char statustopic[100] = "";

String scrollText = "Start...";  //Default Laufschrift
String imageMsg = "";

int pinCS = D8;                          //Für den PIN Select habe ich GPIO0 gewählt
int numberOfHorizontalDisplays = 4;     //number der Module Horizontal
int numberOfVerticalDisplays = 1;       //number der Module Vertikal

int wait = 50;                  //Zeit in ms für Scroll Geschwindigkeit wo gewartet wird
int brightness = 3;             //Default brightness 0 bis 15
int spacer = 1;                 //leer Zeichen länge
int width = 5 + spacer;         //Schriftlänge ist 5 Pixel

long lastMsg = 0;
char msg[100];
int value = 0; 
String string1 = "";
char char1[75];

bool colon = 0;
unsigned long colonPreviousMillis = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 10000; 
const long colonInterval = 1000;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
WiFiClient espClient;
PubSubClient client(espClient);


void local_yield()
{
  yield();
  client.loop();
}


void local_delay(unsigned long millisecs)
{
  unsigned long start = millis();
  local_yield();
  if (millisecs > 0)
  {
    while ((millis()-start) < millisecs)
    {
      local_yield();
    }
  }
}



// WIFI SETUP FUNCTION
/////////////////////////////////////////////////////////////
void setup_wifi() {
  
  delay(10);
WiFiManager wifiManager;
  wifiManager.autoConnect("MQTTDisplay");
 
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  scrollText = WiFi.localIP().toString();
  istext = true;
  newmsg = true;
}



// RECONNECT FUNCTION
/////////////////////////////////////////////////////////////
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      char powertopic[100];
      char messagetopic[100];
      char imagetopic[100];
      char imagerawtopic[100];
      String powertop = "tele/" + mqttname + "/POWER";
      powertop.toCharArray(powertopic, powertop.length()+1);
      String messagetop = "cmnd/" + mqttname + "/MESSAGE";
      messagetop.toCharArray(messagetopic, messagetop.length()+1);
      statustop.toCharArray(statustopic, statustop.length()+1);
      String imagetop = "cmnd/" + mqttname + "/IMAGE";
      imagetop.toCharArray(imagetopic, imagetop.length()+1);
      String imagerawtop = "cmnd/" + mqttname + "/IMAGE_RAW";
      imagerawtop.toCharArray(imagerawtopic, imagerawtop.length()+1);
      client.publish(powertopic, "ON");
      // ... and resubscribe
      client.subscribe(messagetopic);
      client.subscribe(imagetopic);
      client.subscribe(imagerawtopic);
      scrollText = WiFi.localIP().toString();

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



// CALLBACK FUNCTION (TRIGGERED ON MQTT INCOMING)
/////////////////////////////////////////////////////////////
void callback(char* topic, byte* payload, unsigned int length) {
    char msg[length+1];
    for (int i = 0; i < length; i++) {
        msg[i] = (char)payload[i];
    }

    msg[length] = '\0';
    Serial.println();
    Serial.print("MQTT empfangen: ");
    Serial.print(msg);
    Serial.println();
    String topicstr = topic;

    if (topicstr == "cmnd/" + mqttname + "/MESSAGE") {                      // TEXT-NACHRICHT EINGEGANGEN
      Serial.println("Text message detected.");
      Serial.println();
      scrollText = msg;
      istext = true;
    } 

    if (topicstr == "cmnd/" + mqttname + "/IMAGE_RAW") {                    // BILD-ROHDATEN EINGEGANGEN
      Serial.println("Raw image data detected.");
      Serial.println();
      scrollText = msg;
      isimage = true;   
    } 
      
    if (topicstr == "cmnd/" + mqttname + "/IMAGE") {                        // BILD EINGEGANGEN
      scrollText = msg;
      if (scrollText.indexOf(',') != -1) { 
        Serial.println("Image recognized with text message.");
        Serial.println();
        imageMsg = scrollText.substring(scrollText.indexOf(',')+1, scrollText.length()); 
        scrollText = scrollText.substring(0, scrollText.indexOf(','));
        isboth = true;
      }
      Serial.println("Image detected.");
      Serial.println();
      if (scrollText == "home") { scrollText = "0001000000101000010001001000001010111010101110101011101011111110"; }
      if (scrollText == "bell") { scrollText = "0001000000101000010001000100010001000100100000100111110000010000"; }   
      if (scrollText == "fire") { scrollText = "0000100001011001010110111111111111110111011011100110111000111100"; } 
      if (scrollText == "mail") { scrollText = "0000000011111111110000111010010110011001100000011111111100000000"; }  
      if (scrollText == "wash") { scrollText = "0111111001110010010000100101101001100110011001100101101001111110"; }  
      if (scrollText == "yes") { scrollText = "0000000100000011000001110000111011011100111110000111000000100000"; }   
      if (scrollText == "no") { scrollText = "1100001101100110001111000001100000111100011001101100001100000000"; }   
      if (scrollText == "smile") { scrollText = "0011110001000010101001011000000110100101100110010100001000111100"; }
      if (scrollText == "sad" ) { scrollText = "0011110001000010101001011000000110011001101001010100001000111100"; }
      isimage = true;
      
    } 

    newmsg = true;
}

// TEXT show FUNCTION
/////////////////////////////////////////////////////////////
void show() {
  Serial.println();
  Serial.println("Start to display text... ");
  for ( int i = 0 ; i < width * scrollText.length() + matrix.width() - 1 - spacer; i++ ) {
    matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; //Zentrieren des Textes Vertikal
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < scrollText.length() ) {
        matrix.drawChar(x, y, scrollText[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
    matrix.write(); 
    local_delay(wait);    
  }
  Serial.println("Finished!");
  Serial.println();
}

// DRAWIMG FUNCTION
/////////////////////////////////////////////////////////////
void drawimg(String msg) {
    Serial.println();
    Serial.println("Start to draw picture... ");
    matrix.fillScreen(LOW);
    matrix.write();
    for (int row = 0 ; row < 8 ; row++ ) {           // Reihen
      for (int col = 0 ; col < 8 ; col++ ) {           // Spalten
        int pointer = (row*8)+col;
        if (msg.charAt(pointer) == '1') { 
          matrix.drawPixel(col, row, HIGH);
          matrix.drawPixel(col+24,row,HIGH);
        } else {
          matrix.drawPixel(col, row, LOW);
          matrix.drawPixel(col+24,row, LOW);
        }
      }   
    }
    matrix.write(); 
    local_delay(2000);
    Serial.println("Picture drawn!");
    Serial.println();
}

// BLINK FUNCTION
/////////////////////////////////////////////////////////////
void blinken(int number, int duration) {
  Serial.println();
  Serial.println("Start to blink... ");
  for ( int i = 0 ; i < number; i++ ) {
    matrix.fillScreen(HIGH);
    matrix.write();
    local_delay(duration);
    matrix.fillScreen(LOW);
    matrix.write();
    local_delay(duration);
  }
  Serial.println("Finished!");
  Serial.println();
}

// GLOW FUNCTION
/////////////////////////////////////////////////////////////
void glow(int duration) {
  Serial.println();
  Serial.println("Beginne, glow... ");
  matrix.fillScreen(HIGH);
  matrix.write();
  local_delay(duration);
  matrix.fillScreen(LOW);
  matrix.write();
  Serial.println("Finished!");
  Serial.println();
}

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  strcpy(mqtt_server, (const char*)json["mqtt_server"]);
  strcpy(mqttnametemp,(const char*)json["mqttname"]);
  mqttname = String(mqttnametemp);
  strcpy(host,(const char*)json["host"]);
statustop = "tele/" + mqttname + "/STATUS";

  return true;
}

// SETUP FUNCTION
/////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);

  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
yield();
  if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  matrix.setIntensity(brightness); //brightness von Display auf Default Wert einstellen
// Adjust to your own needs
  matrix.setPosition(0, 3, 0); // The first display is at <0, 0>
  matrix.setPosition(1, 2, 0); // The second display is at <1, 0>
  matrix.setPosition(2, 1, 0); // The third display is at <2, 0>
  matrix.setPosition(3, 0, 0); // And the last display is at <3, 0>
  matrix.setRotation(0, 3);        //Erste DOT Matrix Drehen
  matrix.setRotation(1, 3);        //Zweite DOT Matrix Drehen
  matrix.setRotation(2, 3);        //Dritte DOT Matrix Drehen
  matrix.setRotation(3, 3);        //Vierte DOT Matrix Drehen

  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  udp.begin(localPort);

  //Arduino OTA update
   ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  
  getTime();
}



// LOOP FUNCTION
/////////////////////////////////////////////////////////////
void loop() {
  
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
  getTime();
}

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  httpServer.handleClient();
 
  if (newmsg == true) {
    client.publish(statustopic, "BUSY");
    glow(500);
    blinken(5, 100);

    if (istext == true) { show(); }
    if ((isimage == true) || (isboth == true)) { drawimg(scrollText); }
    matrix.fillScreen(LOW);
    matrix.write();
    local_delay(500);
    blinken(2, 250);
    if (istext == true) { show(); }
    if ((isimage == true) && (isboth == false)) { drawimg(scrollText); }
    if ((isimage == true) && (isboth == true)) { scrollText = imageMsg; show(); }
    matrix.fillScreen(LOW);
    matrix.write();  
    local_delay(500);  
    glow(1000);
    isimage=false;
    isboth=false;
    istext=false;
    newmsg=false;
    imageMsg="";
    scrollText="";
    client.publish(statustopic, "IDLE");
  } 
  ArduinoOTA.handle();
  displayTime(); 
  delay(1000);
  scrollText = "Video Recording";
  show();
  displayTime();
  delay(1000);
  //scrollText = "in Progress....";
  //show();
  
}

void getTime() {
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  } else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears + TIMEOFFSET;
    // print Unix time:
    Serial.println(epoch);

hours = (epoch %86400L)/3600;
minutes = (epoch % 3600) / 60;
seconds = (epoch % 60);

  }
}

void displayTime() {
  if (AMPM) {
  if (hours >= 13) { hours = hours - 12; }
 
  if (hours == 0) {
    hours = 12;
  }
}

char temp[10];
unsigned long currentMillis = millis();
if (currentMillis - colonPreviousMillis >= colonInterval) {
    // save the last time you blinked the LED
    colonPreviousMillis = currentMillis;
colon = !colon;
}

if (colon == 1) {
sprintf(temp,"%02d:%02d",hours, minutes);
} else { sprintf(temp,"%02d %02d",hours, minutes); }
temp1 = (String)temp;

    matrix.fillScreen(LOW);

        matrix.drawChar(0, 0, temp1[0], HIGH, LOW, 1);
        matrix.drawChar(6, 0, temp1[1], HIGH, LOW, 1);
        matrix.drawChar(12, 0, temp1[2], HIGH, LOW, 1);
        matrix.drawChar(18, 0, temp1[3], HIGH, LOW, 1);
        matrix.drawChar(24, 0, temp1[4], HIGH, LOW, 1);
        matrix.drawChar(30, 0, temp1[5], HIGH, LOW, 1);
 matrix.write();

}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
