//www.robotedu.my//
#include <ESP8266WiFi.h>
WiFiServer server(80);
IPAddress HTTPS_ServerIP;
void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
  WiFi.softAP("GoouuuMiniS1", ""); // Provide the (SSID, password); . 
  server.begin(); // Start the HTTP Server
  
  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
  
  HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
  Serial.print("Server IP is: "); // Print the IP to the monitor window 
  Serial.println(HTTPS_ServerIP);

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) { 
    return; 
  } 
//  Looking under the hood 
  Serial.println("Somebody has connected :)");

  //Read what the browser has sent into a String class and print the request to the monitor
  String request = client.readStringUntil('\r');
  //Looking under the hood 
  //Serial.println(request);

}
