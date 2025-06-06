#include <WiFi.h>
#include "src/google-home-notifier/esp8266-google-home-notifier.h"

const char* ssid     = "";
const char* password = "";

GoogleHomeNotifier ghn;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("");
  Serial.print("connecting to Wi-Fi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
  
  const char displayName[] = "all"; //"all";

  Serial.println("connecting to Google Home...");
  if (ghn.device(displayName, "en") != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  Serial.print("found Google Home(");
  Serial.print(ghn.getIPAddress());
  Serial.print(":");
  Serial.print(ghn.getPort());
  Serial.println(")");
  
  if (ghn.notify("OK Google, Make the desklight blue") != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  Serial.println("Done.");
}

void loop() {
  // put your main code here, to run repeatedly:

}
