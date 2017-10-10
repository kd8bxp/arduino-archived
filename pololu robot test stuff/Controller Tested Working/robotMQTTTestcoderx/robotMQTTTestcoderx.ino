/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SSD1306.h"
#include <ArduinoJson.h>

// Update these with values suitable for your network.

const char* ssid = "Motel6";
const char* password = "";
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_server = "test.mosquitto.org";

volatile int m1dir = 0;
volatile int m2dir = 0;
long m1steps, m2steps;
long distanceIN = 0;
String RobotID = "";

SSD1306 display(0x3c, 4,5);// SDA D2, SCL D1

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
 
    uint32_t chipid=ESP.getChipId(); 
    char clientid[16];
    char subclientid[16];
    
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  if ((String)topic == "robothome") {
    for (int i=0; i < length; i++) {
      subclientid[i] = (char)payload[i];
    }
    client.subscribe(subclientid);
    client.unsubscribe("robothome");
  }
  display.clear();
  String msg;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  char msg2[200];
  msg.toCharArray(msg2, 200);
  decodePacket(msg2);
  //display.setFont(ArialMT_Plain_10);
  //display.drawStringMaxWidth(35,13,60,msg); //d1 mini
  //display.display();
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    snprintf(clientid,16,"RobotID%08X",chipid);
    // Attempt to connect
    if (client.connect(clientid)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("robothome", clientid);
      // ... and resubscribe
      client.subscribe("robothome");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void displayPkg() {
  
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(35,13,"In: "); //d1 mini
  display.drawString(50,13,(String)distanceIN);
  display.drawString(35,24,"Ct: ");
  char temp[30];
  snprintf(temp,30,"%d,%d", m1steps,m2steps);
  //display.drawString(50,24, (String)m1steps);
  //display.drawString(52,24, ",");
  //display.drawString(55,24, (String)m2steps);
  display.drawString(50, 24, temp);
  display.drawString(35,35, "Dir: ");
  if (m1dir == 0) {display.drawString(55,35, "S");}
  if (m1dir == -1) {display.drawString(55,35, "R");}
  if (m1dir == 1) {display.drawString(55,35, "F");}
  if (m2dir == 0) {display.drawString(60, 35, ",S");}
  if (m2dir == -1) {display.drawString(60, 35, ",R");}
  if (m2dir == 1) {display.drawString(60, 35, ",F");}
  display.drawString(35, 46, RobotID.substring(5));
  display.display();
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  /*long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(clientid, msg);
  }*/
}

void decodePacket(char* msg1) {
  StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(msg1);
Serial.println(msg1);
if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
      }

RobotID = root["RobotID"].asString();
m1dir = root["direction"][0];
m2dir = root["direction"][1];
m1steps = root["encoder"][0];
m2steps = root["encoder"][1];
distanceIN = root["distance"];
displayPkg();
}

