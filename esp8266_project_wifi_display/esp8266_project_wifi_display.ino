/**
 * Project WiFi Display
 * 
 *  This project is intends to enable WiFi Connectivity for the LED Panel
 *  In order to do, a WiFi Accesspoint configuration is used.
 *  The WiFi Access point serves a Dynamic Page where the text to 
 *  be sent can be written and the same would passed on through
 *  Serial Port on ESP8266.
 *  This program was tested on a NodeMCU V1 with ESP-12E module.
 *  There are two serial ports being used the `Serial` which is
 *  the built In one and the other is `SoftSerial` which is an emulated
 *  Software serial port. This would help to make the program compatible
 *  for even the ESP-01 module where only 1 serial port is used.
 *  Else using a ESP-12F/-12E/WROOM2 etc the SoftSerial can be used.
 *  
 *  The WiFi Access point has a Dynamic all pointing DNS feature that 
 *  helps to make the portal directly on the mobile phone or PC.
 *  This protal feature make the program look like a App.
 *  On a PC it would automatically redirect all Domains to the default
 *  first page.
 *  Upon successfully sending the data the Server also displays an 
 *  success message page.
 * 
 * 
 * @author Boseji &lt; salearj [at] hotmail [dot] com &gt;
 * 
 * @version 1.0 First Release (17th July 2016)
 * 
 * @license Attribution-ShareAlike CC BY-SA
 * This Software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* FOUND AT: https://gist.github.com/boseji/4f1fad00b62453113c5da9edb4b6e32a
 */  
 
//////////////////////////////////////////////////////////////////////////
////// INCLUDES
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
//////////////////////////////////////////////////////////////////////////
////// CONSTANTS
// Accesspoint Configuration
const char *AP_ssid = "WiFi-Display";
const char *AP_password = "thereisnospoon";
// DNS Server Port
const byte DNS_PORT = 53;
/////////////////////
// HTTP Page - Max Field "160" Bytes - Change this if neded
#define MAX_INPUTLENTH 160
//  - If nchange is needed change [maxlength=\"<Length>\"] in the Form
//    Eg. For 320 Characters length it would become [maxlength=\"320\"]
/////////////////////
const char HTTP_PAGE[] = {"<!DOCTYPE html><html><head><title>WiFi Display</title>"
  "<style>body{font-family: Verdana, Geneva, Tahoma, sans-serif;"
  "margin: 50px;}.sal{color:#999999;font-style: oblique;}</style></head><body><h1>WiFi Display</h1>"
  "<p class=\"sal\">Designed By <strong>boseji </strong> &lt; salearj [at] hotmail [dot] com &gt; </p>"
};
const char HTTP_BOT[] = {
  "<h2> Text Configuration: </h2>"
  "<form action=\"serial\" method=\"POST\">"
  "<p><input type=\"text\" name=\"SerialData\" placeholder=\"Enter Text to be sent\" maxlength=\"160\"></p>"
  "<input type=\"submit\" value=\"Submit\">"
  "</form></body></html>"
};
//////////////////////////////////////////////////////////////////////////
////// GLOBALS
// Webserver to host the configuration Page
ESP8266WebServer webServer(80);
// External Test Soft-Serial Port on 
//   GPIO14(RX)[NodeMCU Pin D5] and 
//   GPIO12(TX)[NodeMCU Pin D6]
//   Buffer Size of 256
SoftwareSerial swSer(14, 12, false, 256);
// DNS Server Instance
DNSServer dnsServer;
// Character Array for Storing the Received Data
char ser_buf[256];
//////////////////////////////////////////////////////////////////////////
////// FUNCTION DECLARTION
void h_Main(void);
void h_Serial(void);
void h_NotFound(void);
//////////////////////////////////////////////////////////////////////////
////// SETUP
void setup() {
  // Bootup Wait
  delay(1000);
  
  // Setup the Serial Ports
  Serial.begin(9600);
  swSer.begin(9600);
  // Flush Buffer  
  Serial.println();  
  swSer.println();

  // IP Address for Assignments : Server at 192.168.1.1
  IPAddress apIP(192, 168, 1, 1);

  // Setup the AP
  delay(100);
  WiFi.disconnect(false);  
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_ssid, AP_password);
  delay(100);

  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  dnsServer.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  // start DNS server for all Domains to same page
  dnsServer.start(DNS_PORT, "*", apIP);

  // Handle Main Server on Root
  webServer.on("/", h_Main);

  // Handle Post Request on serial
  webServer.on("/serial", h_Serial);
  
  // Redirector for Main Page
  webServer.onNotFound(h_NotFound);

  // Start the Server
  webServer.begin();  
}

//////////////////////////////////////////////////////////////////////////
////// LOOP
void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
//////////////////////////////////////////////////////////////////////////
////// FUNCTION DEFINITIONS
void h_Main(void)
{
  String Content;
  // Main HTML Content
  Content += HTTP_PAGE;
  Content += HTTP_BOT;
  
  // Send out the Page
  webServer.send(200, "text/html", Content);
}
void h_NotFound(void)
{
  String srvpath;  
  // Get the Actual Server Path
  srvpath = String("http://") + WiFi.softAPIP().toString();
  
  webServer.sendHeader("Location", srvpath);
  webServer.send ( 302, "text/plain", "");
  webServer.client().stop();
}
void h_Serial(void)
{ 
  String srvpath;
      
  // Get the Actual Server Path
  srvpath = String(F("http://")) + WiFi.softAPIP().toString();
     
  // Only for POST with Correct Arguments
  if(webServer.hasArg("SerialData") && 
    webServer.method() == HTTP_POST)
  {
    // Get Argument Array
    webServer.arg("SerialData").toCharArray(ser_buf, MAX_INPUTLENTH);
    // Check if there is Data
    if(strlen(ser_buf) != 0)
    {
      // Response HTML Content
      String RContent;
      // Prepare Main Content
      RContent += HTTP_PAGE;
      RContent += F("<h2>WiFi Display : Successfully Sent</h2>");
      RContent += F("<h3>");
      RContent += ser_buf;
      RContent += F("</h3><br><a href=\"");
      RContent += srvpath;
      RContent += F("\">BACK</a>");
      RContent += F("</body></html>");
      webServer.send(200, "text/html", RContent);
      // Send data to Serial
      Serial.println(ser_buf);
      swSer.println(ser_buf);
      return; // End Here
    }
  }
  
  // Here only in Case of Error
  {
    // In all error Cases Redirect to Main Page
    webServer.sendHeader("Location", srvpath);
    webServer.send ( 302, "text/plain", "");
    webServer.client().stop();
  }
}
