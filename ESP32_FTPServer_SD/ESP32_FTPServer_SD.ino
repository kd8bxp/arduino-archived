#include <WiFi.h>
#include <WiFiClient.h>
#include "ESP32FtpServer.h"
#include "SH1106Wire.h" // alias for `#include "SSD1306Wire.h"`
#include "SH1106.h" //https://github.com/ThingPulse/esp8266-oled-ssd1306

const char* ssid = "";
const char* password = "";

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial
SH1106 display(0x3c, 21, 22); 

void setup(void){
  Serial.begin(9600);
    
  WiFi.begin(ssid, password);
  Serial.println("");
display.init();
display.setFont(ArialMT_Plain_10);
display.clear();
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
display.clear();
display.drawString(32,0,"FTP Server");
display.drawString(0,10,"IP: ");
//display.drawString(16, 10, (String)WiFi.localIP());
display.drawString(16,10, WiFi.localIP().toString().c_str());
display.drawString(0,20,"Username: esp32");
display.drawString(0,30,"Password: esp32");
//display.display();
  /////FTP Setup, ensure SD is started before ftp;  /////////
  
  if (SD.begin(5)) {
      Serial.println("SD opened!");
       display.drawString(0,53,"Ready....");
      display.display();
      ftpSrv.begin("esp32","esp32");    //username, password for ftp.  set ports in ESP32FtpServer.h  (default 21, 50009 for PASV)
  }   else {
    Serial.println("SD Not Ready.");
    display.drawString(0,53,"Error - Reboot");
    display.display();
    delay(5000);
    ESP.restart(); 
  }
}

void loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   
}
