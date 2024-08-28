#include "src/M5StickC/M5StickC.h"
#include "src/PubSubClient/PubSubClient.h"
#include "Wire.h"
#include "WiFi.h"

#define JOY_ADDR 0x38

const char* ssid = "ssid";
const char* password = "pass";
const char* mqtt_server = "broker.mqtt-dashboard.com";

#define SUBSCRIBETOPIC "kd8bxp/MQTTBot/OUT"
#define PUBLISHTOPIC "kd8bxp/MQTTBot/IN"

int8_t x_data, y_data, button_data;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String messageIn;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  messageIn = "";
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    messageIn+=(char)payload[i];
  }
  //Serial.println();
  //Serial.println("Color: " + color);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    uint32_t chipid;
    for(int i=0; i<17; i=i+8) {
    chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
    char clientid[25];
    snprintf(clientid,25,"MQTTBot-%08X",chipid);
    if (client.connect(clientid)) {
            client.subscribe(SUBSCRIBETOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
    // initialize the display
    M5.begin();
    Wire.begin(0, 26, 100000UL);
    // clear the background
    M5.Lcd.setRotation(3);
    M5.Lcd.fillRect(0, 0, 80, 160, BLACK);
    M5.Lcd.setCursor(20,0);
    M5.Lcd.print("MQTTBot Controler");
    setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
   
}

void loop() {
    Wire.beginTransmission(JOY_ADDR);
    Wire.write(0x02);
    Wire.endTransmission();
    Wire.requestFrom(JOY_ADDR, 3);
    if (Wire.available()) {
        x_data      = Wire.read();
        y_data      = Wire.read();
        button_data = Wire.read();
    }

       M5.update();
      if (!client.connected()) {
            reconnect();
  }
  client.loop();

  M5.Lcd.setCursor(10,20);
  M5.Lcd.printf("X: %d   ", -x_data); //negitive because of orintation of device
  M5.Lcd.setCursor(60,20);
  M5.Lcd.printf("Y: %d   ", y_data);
  M5.Lcd.setCursor(20,35); 
  M5.Lcd.printf("Button: %d", button_data);
  char cstr[20];
  sprintf(cstr, "%d,%d", -x_data, y_data);
  client.publish(PUBLISHTOPIC, cstr);
  
}
