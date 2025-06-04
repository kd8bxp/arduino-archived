#include <BleKeyboard.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

// WiFi credentials
#define  SSID      "ESP32-S3-GEEK"     // Fill in your WIFI name
#define  PASSWORD  "Waveshare"         // Fill in your WIFI password

// MQTT configuration
const char*  MQTT_SERVER = "mqtt.waveshare.cloud";  
const int    MQTT_PORT   = 1883;

const char*  Client_ID   = "605d54a7";    // Defining device ID
const char*  Sub_Topic   = "Sub/68/54/605d54a7";    // MQTT subscribe to topics

// BLE keyboard configuration
BleKeyboard bleKeyboard("ESP32-S3-GEEK", "Waveshare", 100);

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// Function prototypes
void setupWiFi();
void setupMQTT();
void setupBLE();
void reconnect();
void Screen_ON();
void Screen_OFF();
void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);

  //Initialize the LCD screen
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(100);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_SetRotate(90);
  LCD_Clear(BLACK);
  
  // Setup components
  setupWiFi();
  setupMQTT();
  setupBLE();
}

void loop() {
  // Wait for BLE keyboard connection
  while (!bleKeyboard.isConnected()) {
    delay(500);
    Serial.print(".");
  }

  // Reconnect to MQTT if necessary
  if (!client.connected()) {
    reconnect();
  }
  
  // Handle MQTT messages
  client.loop();
}

// Function to initialize WiFi connection
void setupWiFi() {
  Paint_DrawString_EN(20, 50, "Wifi Connecting...", &Font20, BLACK, GREEN);
  Serial.print("Connecting to WiFi: ");
  WiFi.setSleep(true);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 50, "Wifi Connected", &Font20, BLACK, GREEN);
  Serial.println("\nWiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

// Function to initialize MQTT connection
void setupMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

// Function to initialize BLE keyboard
void setupBLE() {
  bleKeyboard.begin();
  Serial.println("BLE keyboard initialized");
}

// Message callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  String payloadString = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    payloadString += (char)payload[i];
  }

  int keyPosition = payloadString.indexOf("\"key\"");                           // Locate to "key",If your identifier is not "key", change it to your own!
  char keyChar1 = payloadString.charAt(keyPosition + (strlen("\"key\"")+1));    // extract the first digit of the "key" value
  // char keyChar2 = payloadString.charAt(keyPosition + (strlen("\"key\"")+2)); // If the extracted value is greater than one digit, add another digit

  if (keyChar1 == '1') Screen_ON();
  else Screen_OFF();
}

void reconnect() {
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 50, "MQTT Connecting...", &Font20, BLACK, GREEN);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(Client_ID)) {
      Serial.println("Connected to MQTT");
      client.subscribe(Sub_Topic);
    } else {
      Serial.println("Failed, rc=" + String(client.state()) + " trying again in 5 seconds");
      delay(5000);
    }
  }
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 50, "MQTT Connected", &Font20, BLACK, GREEN);
}

void Screen_ON(){
  LCD_Clear(BLACK);
  Paint_DrawString_EN(15, 20, "open screen:", &Font24, BLACK, GREEN);
  Paint_DrawString_EN(15, 65, "Enter          ", &Font20, BLACK, GREEN);
  Serial.println("Screen ON");
  delay(100);
  Serial.println("Enter");
  bleKeyboard.write(KEY_RETURN);    // Enter
  delay(1000);
  Paint_DrawString_EN(15, 65, "Waveshare          ", &Font20, BLACK, GREEN);
  bleKeyboard.print("Waveshare");   // Keyboard output "Waveshare", please change it to your open screen password!
  delay(500);
  bleKeyboard.write(KEY_RETURN);    // Enter
}

void Screen_OFF(){
  LCD_Clear(BLACK);
  Paint_DrawString_EN(15, 20, "close screen:", &Font24, BLACK, GREEN);
  Paint_DrawString_EN(15, 65, "Ctrl+Alt+Delete", &Font20, BLACK, GREEN);
  Serial.println("Screen OFF");
  bleKeyboard.press(KEY_LEFT_CTRL); // Ctrl
  bleKeyboard.press(KEY_LEFT_ALT);  // Alt
  bleKeyboard.press(KEY_DELETE);    // Delete
  delay(100);
  bleKeyboard.releaseAll();         // Press together
  delay(500);
  Paint_DrawString_EN(15, 65, "Enter              ", &Font20, BLACK, GREEN);
  Serial.println("Enter");
  bleKeyboard.write(KEY_RETURN);    // Enter
}