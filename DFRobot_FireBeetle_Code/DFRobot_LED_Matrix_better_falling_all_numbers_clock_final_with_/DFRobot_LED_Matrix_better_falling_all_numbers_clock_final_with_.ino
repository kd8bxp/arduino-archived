/*
Copyright (c) 2018 LeRoy Miller

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

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/

#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          
#endif

//needed for library
#include <DNSServer.h>  //https://github.com/bbx10/DNSServer_tng
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h> //https://github.com/bbx10/WebServer_tng
#endif
#include <WiFiManager.h>         //https://github.com/bbx10/WiFiManager/tree/esp32

#include <NTPClient.h>  //https://github.com/arduino-libraries/NTPClient
#include "DFRobot_HT1632C.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

#if defined( ESP_PLATFORM ) || defined( ARDUINO_ARCH_FIREBEETLE8266 )  //FireBeetle-ESP32 FireBeetle-ESP8266
#define DATA D6
#define CS D2
#define WR D7
//#define RD D8
#else
#define DATA 6
#define CS 2
#define WR 7
//#define RD 8
#endif

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
WiFiServer server(80);

#define TIMEOFFSET -14400 //Find your Time Zone off set Here https://www.epochconverter.com/timezones OFF Set in Seconds
bool AMPM = 1; //1 = AM PM time, 0 = MILITARY/24 HR Time
bool DISPLAYZERO = 0; //display leading zero in zero 0 = No, 1 = Yes

int hours,minutes;
int slideTime = 5; //fast
int updateDelay = 5; //about 10 seconds
static int taskCore = 0;
int tempDigit1, tempDigit2, tempDigit3, tempDigit4;

// Client variables 
char linebuf[80];
int charcount=0;

uint8_t COLONON[] = {
  B00000000,
  B00010100,
  B00000000,
  B00000000
}; //:

uint8_t COLONOFF[] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

//zero
 uint8_t zerobyte0[] = {B01111110,B00000000,B00000000,B00000000};
 uint8_t zerobyte1[] = {B10110001,B00000000,B00000000,B00000000};
 uint8_t zerobyte2[] = {B10001101,B00000000,B00000000,B00000000};
 uint8_t zerobyte3[] = {B01111110,B00000000,B00000000,B00000000};

//one
 uint8_t onebyte0[] = {B01000001,B00000000,B00000000,B00000000};
 uint8_t onebyte1[] = {B11111111,B00000000,B00000000,B00000000};
 uint8_t onebyte2[] = {B00000001,B00000000,B00000000,B00000000};
 uint8_t onebyte3[] = {B00000000,B00000000,B00000000,B00000000};

//two
 uint8_t twobyte0[] = {B01000011,B00000000,B00000000,B00000000};
 uint8_t twobyte1[] = {B10000101,B00000000,B00000000,B00000000};
 uint8_t twobyte2[] = {B10001001,B00000000,B00000000,B00000000};
 uint8_t twobyte3[] = {B01110001,B00000000,B00000000,B00000000};

//three
 uint8_t threebyte0[] = {B01000010,B00000000,B00000000,B00000000}; 
 uint8_t threebyte1[] = {B10001001,B00000000,B00000000,B00000000};
 uint8_t threebyte2[] = {B10001001,B00000000,B00000000,B00000000};
 uint8_t threebyte3[] = {B01110110,B00000000,B00000000,B00000000};

 //four
uint8_t fourbyte0[] = {B00011100,B00000000,B00000000,B00000000};
uint8_t fourbyte1[] = {B00100100,B00000000,B00000000,B00000000};
uint8_t fourbyte2[] = {B01001111,B00000000,B00000000,B00000000};
uint8_t fourbyte3[] = {B10000100,B00000000,B00000000,B00000000};

 //five
uint8_t fivebyte0[] = {B11110001,B00000000,B00000000,B00000000};
uint8_t fivebyte1[] = {B10010001,B00000000,B00000000,B00000000};
uint8_t fivebyte2[] = {B10010001,B00000000,B00000000,B00000000};
uint8_t fivebyte3[] = {B10001110,B00000000,B00000000,B00000000};

 //six
 uint8_t sixbyte0[] = {B01111110,B00000000,B00000000,B00000000};
 uint8_t sixbyte1[] = {B10001001,B00000000,B00000000,B00000000};
 uint8_t sixbyte2[] = {B10001001,B00000000,B00000000,B00000000};
 uint8_t sixbyte3[] = {B01000110,B00000000,B00000000,B00000000};

//seven
uint8_t sevenbyte0[] = {B10000000,B00000000,B00000000,B00000000};
uint8_t sevenbyte1[] = {B10000111,B00000000,B00000000,B00000000};
uint8_t sevenbyte2[] = {B10011000,B00000000,B00000000,B00000000};
uint8_t sevenbyte3[] = {B11100000,B00000000,B00000000,B00000000};

//eight
uint8_t eightbyte0[] = {B01110110,B00000000,B00000000,B00000000};
uint8_t eightbyte1[] = {B10001001,B00000000,B00000000,B00000000};
uint8_t eightbyte2[] = {B10001001,B00000000,B00000000,B00000000};
uint8_t eightbyte3[] = {B01110110,B00000000,B00000000,B00000000};

//nine
uint8_t ninebyte0[] = {B01110010,B00000000,B00000000,B00000000};
uint8_t ninebyte1[] = {B10001001,B00000000,B00000000,B00000000};
uint8_t ninebyte2[] = {B10001001,B00000000,B00000000,B00000000};
uint8_t ninebyte3[] = {B01111110,B00000000,B00000000,B00000000};

void coreTask( void * pvParameters ){

 while(true) {
    webConfig();
   yield();
 }
}


void setup() {
  Serial.begin(9600);
  xTaskCreatePinnedToCore(
                    coreTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    taskCore);  /* Core where the task should run */
  // put your setup code here, to run once:
  display.begin();
  display.isLedOn(true);
  display.clearScreen();
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  display.print("Connected.....",30);
  timeClient.begin();
  timeClient.setTimeOffset(TIMEOFFSET);
  server.begin();
}

void loop() {
  
 timeClient.update(); 
 hours = timeClient.getHours();
 minutes = timeClient.getMinutes();
  
 display.clearScreen();
if (AMPM) {
  if (hours >= 13) { hours = hours - 12; }
 
  if (hours == 0) {
    hours = 12;
  }
}

 if (hours < 10) {
  if (DISPLAYZERO) {
  displayCase(0,1);
  //tempDigit1 = 0; 
  }
  if (AMPM == 0 && hours == 0) {
    displayCase(0,1);
  }
  displayCase(hours,2);
  tempDigit2 = hours;
 } else {
  int temp = hours/10;
  displayCase(temp,1);
  tempDigit1 = temp;
  temp = hours - (hours/10)*10;
  displayCase(temp,2);
  tempDigit2 = temp;
 }

 if (minutes < 10) {
  displayCase(0,3);
  displayCase(minutes,4);
  tempDigit3 = 0;
  tempDigit4 = minutes;
 } else {
  int temp = minutes/10;
  displayCase(temp,3);
  tempDigit3 = temp;
  temp = minutes - (minutes/10)*10;
  //Serial.println(temp);
  displayCase(temp,4);
  tempDigit4 = temp;
 }
 
 
for (int i=0; i<updateDelay; i++) {
  display.drawImage(COLONOFF,3,8,10,0,0);
  display.writeScreen();
  delay(1000);
  display.drawImage(COLONON,3,8,10,0,0);
  display.writeScreen();
  delay(1000);
  }

if (DISPLAYZERO) { removeCase(0,1); } else {
  if (tempDigit1>0) { removeCase(tempDigit1,1); }
}
removeCase(tempDigit2,2);
removeCase(tempDigit3,3);
removeCase(tempDigit4,4);
delay(250);
  
}

void updateDisplay(uint8_t *DIGIT, int number, int loc) {
  int start, offset;
  if (loc == 1) {start = 0 + number; offset = 0;} //3
  if (loc == 2) {start = 5 + number; offset = 5;} //8
  if (loc == 3) {start = 10 + number; offset = 12;} //13
  if (loc == 4) {start = 15 + number; offset = 17;} //17
  for (int i=start; i<25-number; i++) {
  display.drawImage(DIGIT,4,8,25+offset-i,0,0);
  display.writeScreen();
delay(slideTime);
 }
}

void displayCase(int number, int digit) {
  switch (number) {
    case 0:
    updateDisplay(zerobyte0,0,digit);
    updateDisplay(zerobyte1,1,digit);
    updateDisplay(zerobyte2,2,digit);
    updateDisplay(zerobyte3,3,digit);
    break;
    case 1:
    updateDisplay(onebyte0,0,digit);
    updateDisplay(onebyte1,1,digit);
    updateDisplay(onebyte2,2,digit);
    updateDisplay(onebyte3,3,digit);
    break;
    case 2:
    updateDisplay(twobyte0,0,digit);
    updateDisplay(twobyte1,1,digit);
    updateDisplay(twobyte2,2,digit);
    updateDisplay(twobyte3,3,digit);
    break;
    case 3:
    updateDisplay(threebyte0,0,digit);
    updateDisplay(threebyte1,1,digit);
    updateDisplay(threebyte2,2,digit);
    updateDisplay(threebyte3,3,digit);
    break;
    case 4:
    updateDisplay(fourbyte0,0,digit);
    updateDisplay(fourbyte1,1,digit);
    updateDisplay(fourbyte2,2,digit);
    updateDisplay(fourbyte3,3,digit);
    break;
    case 5:
    updateDisplay(fivebyte0,0,digit);
    updateDisplay(fivebyte1,1,digit);
    updateDisplay(fivebyte2,2,digit);
    updateDisplay(fivebyte3,3,digit);
    break;
    case 6:
    updateDisplay(sixbyte0,0,digit);
    updateDisplay(sixbyte1,1,digit);
    updateDisplay(sixbyte2,2,digit);
    updateDisplay(sixbyte3,3,digit);
    break;
    case 7:
    updateDisplay(sevenbyte0,0,digit);
    updateDisplay(sevenbyte1,1,digit);
    updateDisplay(sevenbyte2,2,digit);
    updateDisplay(sevenbyte3,3,digit);
    break;
    case 8:
    updateDisplay(eightbyte0,0,digit);
    updateDisplay(eightbyte1,1,digit);
    updateDisplay(eightbyte2,2,digit);
    updateDisplay(eightbyte3,3,digit);
    break;
    case 9:
    updateDisplay(ninebyte0,0,digit);
    updateDisplay(ninebyte1,1,digit);
    updateDisplay(ninebyte2,2,digit);
    updateDisplay(ninebyte3,3,digit);
    break;
    default:
    break;
  }
  
}

void webConfig() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>Clock Settings.</h1>");
          client.print("<p>Display 24 Hour Time: ");
          client.print(AMPM ? "No" : "Yes");
          client.print(" Display Leading Zero: ");
          client.print(DISPLAYZERO ? "Yes" : "No");
          //client.print(" People: ");
          //client.println(pplDis ? "Yes" : "No");
          //client.print("<br><br>Speed of Slide Timer: ");
          //client.println(slideTime); 
          //client.print("<br>Update Display Timer: ");
          //client.print(updateDelay * 2);
          //client.println(" seconds.");
          client.println("<br><br>Display: ");
          client.println("<p>24 Hour Time: <a href=\"hour1\"><button>YES</button></a>&nbsp;<a href=\"hour0\"><button>NO</button></a></p>");
          client.println("<p>Display Leading Zero: <a href=\"zero1\"><button>YES</button></a>&nbsp;<a href=\"zero0\"><button>NO</button></a></p>");
          //client.println("<p>Speed of Slide Timer: <input onclick=\"submit\" type=\"range\" min=\"1\" max=\"30\" value=5 class=\"slider\" id=\"slideTime\"></p>");
          //client.println("<p>Update Display Timer: <input type=\"range\" min=\"1\" max=\"30\" value=5 class=\"slider\" id=\"updateDelay\"></p>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /hour1") > 0){
            //Serial.println("LED 1 ON");
            AMPM = 0;
          }
          else if (strstr(linebuf,"GET /hour0") > 0){
            //Serial.println("LED 1 OFF");
            AMPM = 1;
          }
          else if (strstr(linebuf,"GET /zero1") > 0){
            //Serial.println("LED 2 ON");
            DISPLAYZERO = 1;
          }
          else if (strstr(linebuf,"GET /zero0") > 0){
            DISPLAYZERO = 0;
          }
          //else if (strstr(linebuf,"GET /slide") > 0) {
          //slideTime = slide; 
        //}
        //else if (strstr(linebuf,"GET /wait") >0) {
        //updateDelay = wait;
      //}*/
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void updateRemoveDisplay(uint8_t *DIGIT, int number, int loc) {
  int start, offset;
  if (loc == 1) {start = 0 + number; offset = 0;} //3
  if (loc == 2) {start = 5 + number; offset = 5;} //8
  if (loc == 3) {start = 10 + number; offset = 12;} //13
  if (loc == 4) {start = 15 + number; offset = 17;} //17
  for (int i=start; i>-1; i--) {
  display.drawImage(DIGIT,4,8,0+i,0,0);
  if (i == 0) { display.clrLine(0,0,0,7); }
  display.writeScreen();
delay(slideTime);
 }
}

void removeCase(int number, int digit) {
  switch (number) {
    case 0:
    updateRemoveDisplay(zerobyte0,0,digit);
    updateRemoveDisplay(zerobyte1,1,digit);
    updateRemoveDisplay(zerobyte2,2,digit);
    updateRemoveDisplay(zerobyte3,3,digit);
    break;
    case 1:
    updateRemoveDisplay(onebyte0,0,digit);
    updateRemoveDisplay(onebyte1,1,digit);
    updateRemoveDisplay(onebyte2,2,digit);
    updateRemoveDisplay(onebyte3,3,digit);
    break;
    case 2:
    updateRemoveDisplay(twobyte0,0,digit);
    updateRemoveDisplay(twobyte1,1,digit);
    updateRemoveDisplay(twobyte2,2,digit);
    updateRemoveDisplay(twobyte3,3,digit);
    break;
    case 3:
    updateRemoveDisplay(threebyte0,0,digit);
    updateRemoveDisplay(threebyte1,1,digit);
    updateRemoveDisplay(threebyte2,2,digit);
    updateRemoveDisplay(threebyte3,3,digit);
    break;
    case 4:
    updateRemoveDisplay(fourbyte0,0,digit);
    updateRemoveDisplay(fourbyte1,1,digit);
    updateRemoveDisplay(fourbyte2,2,digit);
    updateRemoveDisplay(fourbyte3,3,digit);
    break;
    case 5:
    updateRemoveDisplay(fivebyte0,0,digit);
    updateRemoveDisplay(fivebyte1,1,digit);
    updateRemoveDisplay(fivebyte2,2,digit);
    updateRemoveDisplay(fivebyte3,3,digit);
    break;
    case 6:
    updateRemoveDisplay(sixbyte0,0,digit);
    updateRemoveDisplay(sixbyte1,1,digit);
    updateRemoveDisplay(sixbyte2,2,digit);
    updateRemoveDisplay(sixbyte3,3,digit);
    break;
    case 7:
    updateRemoveDisplay(sevenbyte0,0,digit);
    updateRemoveDisplay(sevenbyte1,1,digit);
    updateRemoveDisplay(sevenbyte2,2,digit);
    updateRemoveDisplay(sevenbyte3,3,digit);
    break;
    case 8:
    updateRemoveDisplay(eightbyte0,0,digit);
    updateRemoveDisplay(eightbyte1,1,digit);
    updateRemoveDisplay(eightbyte2,2,digit);
    updateRemoveDisplay(eightbyte3,3,digit);
    break;
    case 9:
    updateRemoveDisplay(ninebyte0,0,digit);
    updateRemoveDisplay(ninebyte1,1,digit);
    updateRemoveDisplay(ninebyte2,2,digit);
    updateRemoveDisplay(ninebyte3,3,digit);
    break;
    default:
    break;
  }
  
}
