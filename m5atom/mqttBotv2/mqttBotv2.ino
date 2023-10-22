#include "src/M5Atom/M5Atom.h"
#include "AtomMotion.h"
#include "src/PubSubClient/PubSubClient.h"
#include "src/ArduinoJson/ArduinoJson.h"
#include "WiFi.h"

AtomMotion Atom;
int direction = 0; // 0 = stop, 1 = forward, 2 = reverse, 3 = left, 4 = right

const char* ssid = "MySpectrumWiFi68-2G";
const char* password = "loudowl369";
const char* mqtt_server = "broker.mqtt-dashboard.com";

#define SUBSCRIBETOPIC "kd8bxp/MQTTBot/IN"
#define PUBLISHTOPIC "kd8bxp/MQTTBot/OUT"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String messageIn;

//servo angle range 0 ~ 180
//DC motor speed range -127~127
String x, y;
int x_data = 0;
int y_data = 0;

void TaskMotion(void *pvParameters){
  while (1)
  {
    
    
        
    vTaskDelay( 100 / portTICK_RATE_MS );
  Atom.SetMotorSpeed(1, x_data);
  Atom.SetMotorSpeed(2, x_data);
  M5.dis.drawpix(0, 0xFFFF00); 
    /*if(direction == 0){
      //stop
      Atom.SetMotorSpeed(1, 0);
      Atom.SetMotorSpeed(2, 0);
      M5.dis.drawpix(0, 0x00FF00); //red
    }
     else if (direction == 1) {
      //forward
      Atom.SetMotorSpeed(1,100);
      Atom.SetMotorSpeed(2,100);
      M5.dis.drawpix(0, 0xff0000); //green
    }
    else if (direction == 2) { 
      //reverse
      Atom.SetMotorSpeed(1,-65);
      Atom.SetMotorSpeed(2,-65);
      M5.dis.drawpix(0, 0x0000ff); //blue
    }
    else if (direction == 3) {
      //left
      Atom.SetMotorSpeed(1, -60);
      Atom.SetMotorSpeed(2, 50);
      M5.dis.drawpix(0, 0x00eeee);
    }
    else if (direction == 4) {
      //right
      Atom.SetMotorSpeed(1, 50);
      Atom.SetMotorSpeed(2, -50);
      M5.dis.drawpix(0, 0x00eeee);
    }
   */ 
  }
  
  //vTaskDelay(1000/ portTICK_RATE_MS);
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
    //Serial.print("Attempting MQTT connection...");
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
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup(){ 
Serial.begin(115200);
  
  M5.begin(true, true, true);
  Atom.Init();     //sda  25     scl  21 
  
  xTaskCreatePinnedToCore(
    TaskMotion
    ,  "TaskMotion"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);
setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 direction = 0;
 x_data = 0;
 y_data = 0;
 delay(3000);
}

void loop() {
    M5.update();
      if (!client.connected()) {
        direction = 0; //estop if MQTT connection is lost
    reconnect();
  }
  client.loop();
  // 0 = stop, 1 = forward, 2 = reverse, 3 = left, 4 = right

  //if (messageIn == "stop" || messageIn == "s" || messageIn =="S" ||messageIn == "0") {direction = 0;}
  //if (messageIn == "forward" || messageIn == "f" || messageIn == "F" || messageIn == "1") {direction = 1;}
  //if (messageIn == "reverse" || messageIn == "r" || messageIn == "R" || messageIn == "2" || messageIn == "back" || messageIn == "b" || messageIn == "B" ) {direction = 2;}
  //if (messageIn == "left" || messageIn == "l" || messageIn == "L" || messageIn == "3") {direction = 3;}
  //if (messageIn == "right" || messageIn == "r" || messageIn == "R" || messageIn == "4") {direction = 4;}
   char msgIn[20];
  messageIn.toCharArray(msgIn, messageIn.length());
  sscanf(msgIn, "%d,%d", &x_data, &y_data);
  if (x_data != 0) { x_data = map(x_data, -50, 100, -127, 127); } 
  if (y_data != 0) { y_data = map(y_data, -50, 100, -127, 127); }
  //x_data = x.toInt();
  //y_data = y.toInt();
}
