#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

const char* ssid1 = "Motel6";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";
//const char* mqtt_server = "rpi-speak.local";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const long interval = 1000;   
unsigned long previousMillis = 0; 

SSD1306  display(0x3c, 5, 4);

char draw[30];
char ssid[30];
char mac[30];

#define LINE0 3
#define LINE1 13
#define LINE2 23
#define LINE4 33
#define LINE5 43


void setup() {
  
  Serial.begin(9600);
  sub();
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
    
}

void sub() {
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void messageBox(const char *message)
{
  Serial.println(message);
  display.clear();
  display.drawString(0, 5, message);
  display.display();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid1);
  WiFi.disconnect();
  WiFi.begin(ssid1, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  display.clear();
  String msg;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //display.clear();
    msg += (char)payload[i];
  }
   // display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(35,25,msg);
    display.display();
  
  
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266KD8BXPDisplay1Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("Say", "-t 'hello world'");
      // ... and resubscribe
      client.subscribe("display1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void scanner() {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
   //messageBox("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks(false,true); // false - async, true - hidden networks
//messageBox("scan done");
  if (n == 0)
    messageBox("no networks found");
  else
  {
    sprintf(draw, "%d netw. found", n);
    messageBox(draw);
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(") ");
      Serial.println(getEncryptionType(WiFi.encryptionType(i)));

      // oled
      strcpy(ssid,WiFi.SSID(i).c_str());
      int dl=strlen(ssid);
      if(dl>2) { // hide some details
       // ssid[dl-2]='?';
       // ssid[dl-1]='?';
      }
      strcpy(mac,WiFi.BSSIDstr(i).c_str());
      dl=strlen(mac);
      if(dl>2) { // hide some details
       // mac[dl-2]='?';
       // mac[dl-1]='?';
      }
      display.clear();
      sprintf(draw,"Net: %d/%d",i+1,n);
      display.drawString(1, LINE0, draw);
      display.drawString(1, LINE1, ssid);
      display.drawString(1, LINE2, mac);
      sprintf(draw,"Sig: %d dBm",WiFi.RSSI(i));
      display.drawString(1, LINE4, draw);
      sprintf(draw,"Ch: %d",WiFi.channel(i));
      display.drawString(80, LINE4, draw);
      sprintf(draw,"Enc: %s",getEncryptionType(WiFi.encryptionType(i)));
      display.drawString(1, LINE5, draw);
      display.drawRect(0,0,128,64);
      display.display();
      
      delay(2000);
    }
  }
  Serial.println("");
}

const char S_WEP[] = "WEP";
const char S_TKIP[] = "WPA";
const char S_CCMP[] = "WPA2";
const char S_NONE[] = "None";
const char S_AUTO[] = "Auto";
const char S_UNKNOWN[] = "Unkn.";

const char* getEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      return S_WEP;
    case ENC_TYPE_TKIP:
      return S_TKIP;
    case ENC_TYPE_CCMP:
      return S_CCMP;
    case ENC_TYPE_NONE:
      return S_NONE;
    case ENC_TYPE_AUTO:
      return S_AUTO;
    default:
      return S_UNKNOWN;
  }
  
}

void loop() {

unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
       scanner(); } else { 
      previousMillis = currentMillis;
      sub();
    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    }
 
}  
