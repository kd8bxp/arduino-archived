#include <SPI.h>
 
#define SSID "dlink1" // insert your SSID
#define PASS "" // insert your password
#define LOCATIONID "2925533" // location id
#define DST_IP "188.226.224.148" //api.openweathermap.org
#include <SoftwareSerial.h>
SoftwareSerial esp(3,2);  // use pins 7, 8 for software Serial 

// If your MCU has dual hardware USARTs (e.g. ATmega644)
// use the definitions below
#define dbg Serial    // use Serial for debug
//#define esp Serial1   // use Serial1 to talk to esp8266
#define DBG_BAUD_RATE 115200  // hardware Serial can go up to 115200bps

void setup()
{
  
  Serial.println("Init");
  
  esp.println("AT+RST"); // restet and test if module is redy
  delay(1000);
  if(esp.find("ready")) {
    Serial.println("WiFi - Module is ready");
  }else{
    Serial.println("Module dosn't respond.");
    while(1);
  }
  delay(1000);
  // try to connect to wifi
  boolean connected=false;
  for(int i=0;i<5;i++){
    if(connectWiFi()){
      connected = true;
      Serial.println("Connected to WiFi...");
      break;
    }
  }
  if (!connected){
    Serial.println("Coudn't connect to WiFi.");
    while(1);
  }
  delay(5000);
  esp.println("AT+CIPMUX=0"); // set to single connection mode
}
void loop()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  esp.println(cmd);
  Serial.println(cmd);
  if(esp.find("Error")) return;
  cmd = "GET /data/2.5/weather?id=";
  cmd += LOCATIONID;
  cmd += " HTTP/1.0\r\nHost: api.openweathermap.org\r\n\r\n";
  esp.print("AT+CIPSEND=");
  esp.println(cmd.length());
  if(esp.find(">")){
    Serial.print(">");
  }else{
    esp.println("AT+CIPCLOSE");
    Serial.println("connection timeout");
    delay(1000);
    return;
  }
  esp.print(cmd);
  unsigned int i = 0; //timeout counter
  int n = 1; // char counter
  char json[100]="{";
  while (!esp.find("\"main\":{")){} // find the part we are interested in.
  while (i<60000) {
    if(esp.available()) {
      char c = esp.read();
      json[n]=c;
      if(c=='}') break;
      n++;
      i=0;
    }
    i++;
  }
  Serial.println(json);
  Serial.println("====");
  delay(600000);
}
 
boolean connectWiFi()
{
  esp.println("AT+CWMODE=1");
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp.println(cmd);
  delay(2000);
  if(esp.find("OK")){
    Serial.println("OK, Connected to WiFi.");
    return true;
  }else{
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}