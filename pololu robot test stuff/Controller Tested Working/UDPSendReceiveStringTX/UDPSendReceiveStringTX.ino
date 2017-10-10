#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "Count: ";       // a string to send back

WiFiUDP Udp;

int count = 0;

void setup() {
  Serial.begin(9600);
  // start the Ethernet and UDP:
  WiFi.begin("Motel6","");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //10.11.0.83
 
  Udp.begin(localPort);

  
}

void loop() {
  
  count++;

    Udp.beginPacket("10.11.0.83", 8888);
    Udp.write(ReplyBuffer);
    Udp.print(count, DEC);
    Udp.endPacket();
  delay(5000);
}

