

#include <ESP8266WiFi.h>

// replace with your channel’s thingspeak API key,
String apiKey = "G6LL0HUDI6WB7GM5";
const char* ssid = "base83";
const char* password = "D809928B0A1859CF23A76380E1";

const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() {
Serial.begin(115200);
delay(10);

WiFi.begin(ssid, password);

Serial.println();
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

//if (digitalRead(2) == LOW) {
client.connect("http://api.pushingbox.com/pushingbox?devid=v2393937644516EE",80);
/*if (client.connect(server,80)) { // "184.106.153.149" or api.thingspeak.com
String postStr = apiKey;
postStr +="&field1=1";
//postStr += String(t);
postStr +="&field2=1";
//postStr += String(h);
postStr += "\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  
  }
  Serial.println();
  Serial.println("closing connection");

//}
//}
*/
client.stop();

Serial.println("Waiting…");
// thingspeak needs minimum 15 sec delay between updates
delay(20000);

}
void loop() {}
