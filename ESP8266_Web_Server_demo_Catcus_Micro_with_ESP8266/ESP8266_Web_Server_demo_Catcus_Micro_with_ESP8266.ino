/* ====== ESP8266 Demo ======
 *   Print out analog values
 * ==========================
 *
 * Change SSID and PASS to match your WiFi settings.
 * The IP address is displayed to soft serial upon successful connection.
 *
 * Ray Wang @ Rayshobby LLC
 * http://rayshobby.net/?p=9734
 

 */

// comment this part out if not using LCD debug
#include <SoftwareSerial.h>
SoftwareSerial serial1(11, 12); // using pin 7, 8 for software serial

enum {WIFI_ERROR_NONE=0, WIFI_ERROR_AT, WIFI_ERROR_RST, WIFI_ERROR_SSIDPWD, WIFI_ERROR_SERVER, WIFI_ERROR_UNKNOWN};

#define BUFFER_SIZE 128

#define SSID ""
#define PASS ""
#define PORT  "80"      // using port 8080 by default

char buffer[BUFFER_SIZE];

void setup() {
  
pinMode(13,OUTPUT);
digitalWrite(13,HIGH);
delay(1000);
  Serial.begin(9600);
  serial1.begin(9600);
  serial1.setTimeout(5000);
  
  //dbg.begin(9600);
  Serial.println("begin.");
   
  byte err = setupWiFi();
  if (err) {
    // error, print error code
    Serial.print("setup error:");
    Serial.println((int)err);
  } else {
    // success, print IP
    Serial.print("ip addr:");
    char *ip = getIP();
    if (ip) {
      Serial.println(ip);
    }
    else {
      Serial.println("none");
    }
    maxTimeout();
  }
}

bool maxTimeout() {
  // send AT command
  serial1.println("AT+CIPSTO=0");
  if(serial1.find("OK")) {
    return true;
  } else {
    return false;
  }
}

char* getIP() {
  // send AT command
  serial1.println("AT+CIFSR");

  // the response from the module is:
  // AT+CIFSR\n\n
  // 192.168.x.x\n 
  // so read util \n three times
  serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  buffer[strlen(buffer)-1]=0;
  return buffer;
}

void loop() {
  int ch_id, packet_len;
  char *pb;  
  serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);
  if(strncmp(buffer, "+IPD,", 5)==0) {
    // request: +IPD,ch,len:data
    sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
    if (packet_len > 0) {
      // read serial until packet_len character received
      // start from :
      pb = buffer+5;
      while(*pb!=':') pb++;
      pb++;
      if (strncmp(pb, "GET /", 5) == 0) {
        serve_homepage(ch_id);
      }
    }
  }
}

void serve_homepage(int ch_id) {
  String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n";

  String content="";
  // output the value of each analog input pin
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    content += "analog input ";
    content += analogChannel;
    content += " is ";
    content += sensorReading;
    content += "<br />\n";       
  }

  header += "Content-Length:";
  header += (int)(content.length());
  header += "\r\n\r\n";
 
  serial1.print("AT+CIPSEND=");
  serial1.print(ch_id);
  Serial.println(ch_id);
 serial1.print(",");
 
 
 Serial.print("AT+CIPSEND=");
 Serial.print(ch_id);
 Serial.print(",");
 
  int senddata = header.length()+content.length();
 Serial.println(senddata);
  serial1.println(senddata);
  if (serial1.find(">")) {
    serial1.print(header);
     Serial.println(header);
 // Serial.println(header.length());
    serial1.print(content);
     Serial.println(content);
 // Serial.println(content.length());
   //serial1.print("AT+CIPSEND=0,11");
  // serial1.print("Hello World");
    delay(20);
 }
  
  Serial.print("AT+CIPCLOSE=");
  Serial.println(ch_id);
  serial1.print("AT+CIPCLOSE=");
  serial1.println(ch_id);
}

byte setupWiFi() {
  serial1.println("AT");
  if(!serial1.find("OK")) {
    return WIFI_ERROR_AT;
  }
  delay(500); 

  // reset WiFi module
  serial1.println("AT+RST");
  if(!serial1.find("ready")) {
    return WIFI_ERROR_RST;
  }
  delay(500);
 
  // set mode 3
  serial1.print("AT+CWJAP=\"");
  serial1.print(SSID);
  serial1.print("\",\"");
  serial1.print(PASS);
  serial1.println("\"");
  delay(2000);
  if(!serial1.find("OK")) {
    return WIFI_ERROR_SSIDPWD;
  }
  delay(500);
  
  // start server
  serial1.println("AT+CIPMUX=1");
  if(!serial1.find("OK")){
    return WIFI_ERROR_SERVER;
  }
  delay(500);
  
  serial1.print("AT+CIPSERVER=1,"); // turn on TCP service
  serial1.println(PORT);
  if(!serial1.find("OK")){
    return WIFI_ERROR_SERVER;
  }
  delay(500);
  
  return WIFI_ERROR_NONE;
}


// Send AT commands to module
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  serial1.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (serial1.available())
    {
      // The esp has data so display its output to the serial window
      char c = serial1.read(); // read the next character.
      response += c;
    }
  }
  
    Serial.print(response);
  
  return response;
}
