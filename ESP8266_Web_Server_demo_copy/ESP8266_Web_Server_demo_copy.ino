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
//#include <SoftwareSerial.h>
//SoftwareSerial dbg(10, 11); // using pin 7, 8 for software serial

enum {WIFI_ERROR_NONE=0, WIFI_ERROR_AT, WIFI_ERROR_RST, WIFI_ERROR_SSIDPWD, WIFI_ERROR_SERVER, WIFI_ERROR_UNKNOWN};

#define BUFFER_SIZE 128

#define SSID ""
#define PASS ""
#define PORT  "8080"      // using port 8080 by default

char buffer[BUFFER_SIZE];

void setup() {

  Serial1.begin(9600);
  Serial1.setTimeout(5000);
  
  Serial.begin(9600);
  Serial.println("begin.");

Serial1.println("AT+RST");
    
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
  Serial1.println("AT+CIPSTO=0");
  if(Serial1.find("OK")) {
    return true;
  } else {
    return false;
  }
}

char* getIP() {
  // send AT command
  Serial1.println("AT+CIFSR");

  // the response from the module is:
  // AT+CIFSR\n\n
  // 192.168.x.x\n 
  // so read util \n three times
  Serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  Serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  Serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);  
  buffer[strlen(buffer)-1]=0;
  return buffer;
}

void loop() {
  int ch_id, packet_len;
  char *pb;  
  Serial1.readBytesUntil('\n', buffer, BUFFER_SIZE);
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
  String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nRefresh: 5\r\n";

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
  Serial1.print("AT+CIPSEND=");
  Serial1.print(ch_id);
  Serial1.print(",");
  Serial1.println(header.length()+content.length());
  if (Serial1.find(">")) {
    Serial1.print(header);
    Serial1.print(content);
    delay(20);
  }
  /*Serial1.print("AT+CIPCLOSE=");
  Serial1.println(ch_id);*/
}

byte setupWiFi() {
  Serial1.println("AT");
  if(!Serial1.find("OK")) {
    return WIFI_ERROR_AT;
  }
  delay(500); 

  // reset WiFi module
  Serial1.println("AT+RST");
  if(!Serial1.find("ready")) {
    return WIFI_ERROR_RST;
  }
  delay(500);
 
  // set mode 3
  Serial1.print("AT+CWJAP=\"");
  Serial1.print(SSID);
  Serial1.print("\",\"");
  Serial1.print(PASS);
  Serial1.println("\"");
  delay(2000);
  if(!Serial1.find("OK")) {
    return WIFI_ERROR_SSIDPWD;
  }
  delay(500);
  
  // start server
  Serial1.println("AT+CIPMUX=1");
  if(!Serial1.find("OK")){
    return WIFI_ERROR_SERVER;
  }
  delay(500);
  
  Serial1.print("AT+CIPSERVER=1,"); // turn on TCP service
  Serial1.println(PORT);
  if(!Serial1.find("OK")){
    return WIFI_ERROR_SERVER;
  }
  delay(500);
  
  return WIFI_ERROR_NONE;
}
