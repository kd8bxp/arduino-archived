
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager




//const char* ssid     = "";
//const char* password = "";
const char* hostname = "Robot1";

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

 startWiFi();
 Serial.println("");

 if (WiFi.status() != WL_CONNECTED) { 
  Serial.println("SoftAP Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
 } else {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } 
}
void loop() {
  // put your main code here, to run repeatedly:

}

void startWiFi() {

WiFiManager wifiManager;
//wifiManager.resetSettings();
wifiManager.setTimeout(120);
//unsigned long previousMillis = millis();
 if(!wifiManager.autoConnect("AutoConnectAP") ) { //&& millis() - previousMillis <=60 * 1000) {
  Serial.println("failed to connect and hit timeout");
    delay(2000);
    //reset and try again, or maybe put it to deep sleep
    //ESP.restart();
    //delay(5000);
  } 
 if (WiFi.status() != WL_CONNECTED) { 
  WiFi.mode(WIFI_AP_STA); 
  WiFi.softAP(hostname);
  }
/*WiFi.mode(WIFI_AP_STA);
  Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    WiFi.softAP(hostname);
unsigned long previousMillis = millis();
//unsigned long currentMillis = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - previousMillis <= 5000) {
        delay(500);
        Serial.print(".");
    } 
    
 if (WiFi.status() != WL_CONNECTED) {  
  WiFi.softAP(hostname);
  }
  */
}
