/*
 *  
 *    http://server_ip/relay#totoggle
 *  http://hostname.local/relay#totoggle
 *  
 *  Sometimes hostname doesn't show up on the network quickly,
 *  pinging the IP address seems to help.
 *  
 *  (C) LeRoy Miller, 2017
 *  
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>


const char* host = "wifirelay";
const char* ssid = "Motel6";
const char* password = "";

//Set All Relays to OFF
boolean toggle1 = 0;
boolean toggle2 = 0;
boolean toggle3 = 0;
boolean toggle4 = 0;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
   Serial.begin(115200);
  delay(10);
 
   pinMode(12, OUTPUT); //D10 
   pinMode(13, OUTPUT); //D8
   pinMode(14, OUTPUT); //D4
   pinMode(16, OUTPUT); //D3
   
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.hostname(host);
  WiFi.begin(ssid, password);
  MDNS.begin(host);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  client.println("Wifi Relay v1.0");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  req.replace("%20", " ");
  req.remove(0,5);
  req.remove(req.length()-9, 9);
  Serial.println(req);
  //client.println(req);
  if (req == "1") {
    client.print("Relay One: ");
    toggle1 = !toggle1;
    if (toggle1 == 0) { 
      client.print("Off"); } else {
        client.print("On");
      }
    digitalWrite(12, toggle1);
  }
  if (req == "2") {
    client.print("Relay Two: ");
    toggle2 = !toggle2;
    if (toggle2 == 0) { 
      client.print("Off"); } else {
        client.print("On");
      }
    digitalWrite(13, toggle2);
  }
  if (req == "3") {
    client.print("Relay Three: ");
    toggle3 = !toggle3;
    if (toggle3 == 0) { 
      client.print("Off"); } else {
        client.print("On");
      }
    digitalWrite(14, toggle3);
  }
  if (req == "4") {
    client.print("Relay Four: ");
    toggle4 = !toggle4;
    if (toggle4 == 0) { 
      client.print("Off"); } else {
        client.print("On");
      }
    digitalWrite(16, toggle4);
  }
  client.flush();
  
  
  Serial.println("Client disonnected");
delay(100);
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

