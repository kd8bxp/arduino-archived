// Compiles with M5Stack Board Core 1.0.9

#include "src/M5ez/M5ez.h"
#include <vector>

#include "src/PubSubClient/PubSubClient.h"

WiFiClient espClient;
PubSubClient client(espClient);

//const char* mqtt_server = "192.168.1.10";
const char* mqtt_server = "broker.mqtt-dashboard.com";
const int mqtt_port = 1883;

//Provide your own WiFi credentials
const char* ssid = "";
const char* password = "";

std::vector<String> msgList;
bool inchat = false;

// on message received
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  String msg = String((char*) payload);
  Serial.println(msg);
  msgList.push_back(msg);
  redraw();  
}

void setup() {
  ez.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(200);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  ezMenu mainmenu("Welcome to M5ez");
  mainmenu.addItem("chat", chat_menu);
  mainmenu.addItem("settings", ez.settings.menu);
  mainmenu.run();
}

// true: connected,  false: not connected
bool keepMqttConn() {
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        String clientId = getMacAddr(); 
        if (client.connect(clientId.c_str())) {
          Serial.print("Connecting done");
          // Once connected, publish an announcement...
          //client.publish("outTopic", "hello world");
          client.subscribe("chat");
          return true;
        } else {
          return false;
        }
    }
    return true;
}

// draw chat messages on LCD
void redraw() {
  if(!inchat) {
    return;
  }
  ez.canvas.clear();
  ez.header.show("chat");
  ez.canvas.font(&FreeSans9pt7b);
  ez.canvas.lmargin(10);

  // show latest 8 messages
  int total = 0;
  for(int i = msgList.size() - 1 ; i >= 0; i--) {
    if(total >= 8) {
      break;
    }
    ez.canvas.println(msgList[i]);
    total++;
  }
}

// chat main
void chat_menu() {
  if(WiFi.status() != WL_CONNECTED) {
    ez.msgBox("notice", "Wi-Fi is not enabled. Please setting Wi-Fi.");
    return;
  }
  inchat = true;
  
  ez.buttons.show("up # Back # input # # down #");
  redraw();
  
  while(true) {
    if(!keepMqttConn()) {
      ez.msgBox("notice", "MQTT server not found.");
      return;
    }
    client.loop();
    
    String btnpressed = ez.buttons.poll(); // check button status
    if(btnpressed == "Back") {
      break;
    }else if(btnpressed == "input") {
      ez.buttons.clear(true);
      ez.canvas.clear();
      
      String msg = ez.textInput();
      msg.trim();

      // send message
      if(msg.length() > 0) {
        //msgList.push_back(msg);
        keepMqttConn();
        client.publish("chat", msg.c_str());
      }
      
      redraw();
      ez.buttons.clear(true);
      ez.buttons.show("up # Back # input # # down #");
    }else if (btnpressed != "") {

    }
  }
  inchat = false;
}

// get MAC address of device
String getMacAddr() {
  uint8_t baseMac[6];
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[16] = {0};
  sprintf(baseMacChr, "%02X%02X%02X%02X%02X%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return String(baseMacChr);
}
