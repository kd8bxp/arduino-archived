#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>

#include <WiFi.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>


#define FIRSTNODE 1 //set to 1 if this is the 1st node
#define PIN 27

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 5, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);


int pixelPerChar = 3; // Width of Standard Font Characters is 8X6 Pixels
int x; // Width of the Display
int i = 0; // Counter
int clr = 0; // Counter for Indexing Array of Colors

const char* ssid = "ssid";
const char* password = "pass";
const char* mqtt_server = "test.mosquitto.org";

//long lastMsg = 0;
//char msg[50];
//int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(matrix.Color(0,0,255));
  matrix.setFont(&TomThumb);
  if (FIRSTNODE == 1) { writeText("HELLO WORLD");  } //set node next node number here
}

void loop() {
  //writeText("HELLO WORLD"); //Print the Message String;
  //delay(1000);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void writeText(String msg1) {
  int arrSize = 1; //arr_len( colors ); // Array of Text Colors;
  int msgSize = (msg1.length() * pixelPerChar) + (2 * pixelPerChar); // CACULATE message length;
  int scrollingMax = (msgSize) + matrix.width(); // ADJUST Displacement for message length;

  x = matrix.width(); // RESET Cursor Position and Start Text String at New Position on the Far Right;
  clr = 0; // RESET Color/Repeat Index;

  while (clr < arrSize) {
    /* Change Color with Each Pass of Complete Message */
    //matrix.setTextColor(colors[clr]);

    matrix.fillScreen(0); // BLANK the Entire Screen;
    matrix.setCursor(x, 5); // Set Starting Point for Text String;
    matrix.print(msg1); // Set the Message String;
//matrix.print("Hello World");
    /* SCROLL TEXT FROM RIGHT TO LEFT BY MOVING THE CURSOR POSITION */
    if (--x < -scrollingMax ) {
      /*  ADJUST FOR MESSAGE LENGTH  */
      // Decrement x by One AND Compare New Value of x to -scrollingMax;
     // This Animates (moves) the text by one pixel to the Left;

      x = matrix.width(); // After Scrolling by scrollingMax pixels, RESET Cursor Position and Start String at New Position on the Far Right;
      ++clr; // INCREMENT COLOR/REPEAT LOOP COUNTER AFTER MESSAGE COMPLETED;
    }
    matrix.show(); // DISPLAY the Text/Image
    delay(100); // SPEED OF SCROLLING or FRAME RATE;
  }
  clr = 0; // Reset Color/Loop Counter;

/* LATHER - RINSE - REPEAT - Why coders have such nice hair */
}

void setup_wifi() {
WiFi.persistent(false);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
     WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
  delay(100);
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
  String msg;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //display.clear();
    msg += (char)payload[i];
  }
     if (msg == "0 Go") { writeText("HELLO WORLD"); client.publish("Display1", "1 Go"); } //set node number to this device, and send a go to next node
   

  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    uint64_t chipid = ESP.getEfuseMac(); // MAC address of ESP32
    uint16_t chip = (uint16_t)(chipid>>32);
    char clientid[25];
    snprintf(clientid,25,"WIFI-Display-%04X",chip);
    if (client.connect(clientid)) {
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
