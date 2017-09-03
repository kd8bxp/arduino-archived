#include <WiFi101.h>
#include <ArduinoCloud.h>

/////// Wifi Settings ///////
char ssid[] = "Motel6";
char pass[] = "";


// Arduino Cloud settings and credentials
const char userName[]   = "";
const char thingName[] = "";
const char thingId[]   = "";
const char thingPsw[]  = "";


WiFiSSLClient sslClient;


// build a new object "newthing"
ArduinoCloudThing newthing;


void setup() {
  Serial.begin (9600);

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
  }


  newthing.begin(thingName, userName, thingId, thingPsw, sslClient);
  newthing.enableDebug();
  // define the properties
  newthing.addProperty("STATUS", CHARSTRING, R);
  
}

void loop() {
  newthing.poll();

  newthing.writeProperty("STATUS", "1");
  
  delay(5000);
  newthing.writeProperty("STATUS", "0");
  
  delay(5000);
}

