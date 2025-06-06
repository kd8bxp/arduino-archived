 /*
  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
*/

//Compiles with ESP32 core 1.0.6

  #include <WiFi.h>
   #include "src/AsyncTCP/AsyncTCP.h"

#include "src/ESPAsyncWebServer/ESPAsyncWebServer.h"
#include "src/WebSerial/WebSerial.h"
#include "src/AsyncElegantOTA/AsyncElegantOTA.h"

const char* ssid = ""; // Your WiFi SSID
const char* password = ""; // Your WiFi Password

AsyncWebServer server(80);

int count;
String rec;


/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "reset") {count = 0;}
  
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  WebSerial.begin(&server);
    /* Attach Message Callback */
    WebSerial.msgCallback(recvMsg);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  count++;
  WebSerial.print(count);
  WebSerial.println(" Hello World....");
  //Serial.println();
  delay(1000);
}
