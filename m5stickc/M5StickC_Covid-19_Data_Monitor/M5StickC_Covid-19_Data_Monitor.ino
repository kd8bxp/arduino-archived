#include "src/M5StickC/M5StickC.h"
#include <Arduino.h> 
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>  
#include "src/ArduinoHttpClient/ArduinoHttpClient.h"

/*____Wifi _____________________*/
#define WIFI_SSID "ssid"       // Enter your SSID here
#define WIFI_PASS "pass"    // Enter your WiFi password here

const int NetworkTimeout = 30*1000;
const int NetworkDelay = 2000; 
double vbat = 0.0;
int discharge, charge;
double temp = 0.0;
double bat_p = 0.0;
double bat_p2 = 0.0;
int status = WL_IDLE_STATUS;
String infected;
String recovered;
String deaths;


WiFiClientSecure client;
HttpClient http(client,"www.worldometers.info", 443); 
uint8_t bat_3[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x00, 0xff,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xdb, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xdb,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
};

uint8_t bat_2[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x00, 0xff,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xdb, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xdb,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
};

uint8_t bat_1[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x00, 0xff,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xdb,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
};



void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  M5.begin();
  M5.Lcd.setRotation(3);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(20, 25);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.printf("Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.setCursor(20, 40);
    M5.Lcd.setTextSize(1);
    M5.Lcd.print("Please Wait...");
    delay(500);
  }
  M5.Lcd.setCursor(15, 25);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.println("Connected");
  IPAddress ip = WiFi.localIP();
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println(ip);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Data Fetching");
  
}

void loop() {
 //check_country("India");
 //delay(2000);
 //check_country("Kuwait");
 //delay(2000);  
 //check_country("Saudi-Arabia");
 //delay(2000); 
 //check_country("Qatar");
 //delay(2000);
 check_country("US");
 delay(10 * 1000);
 //check_country("Italy");
 //delay(2000); 
 //check_country("China");
 //delay(2000); 
 //check_country("Spain");
 //delay(2000); 
 check_country("World");
 delay(10 * 1000);

 
  
}


void draw_country_screen(String sCountry){
  M5.Lcd.fillScreen(BLACK);
  battery_status();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(ORANGE);
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.print(sCountry + ":");

  // infected
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 35);
  //M5.Lcd.printf("Infected: %2d ",infected);
  M5.Lcd.print("Infected: " + infected);
  ////M5.Lcd.setCursor(100, 35);
  ////M5.Lcd.print(infected);
  //Serial.print("Infected:");
  //Serial.print(infected);

  // recovered
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 50);
  //M5.Lcd.printf("Recovered: %02d ",recovered);
  M5.Lcd.print("Recovered: " + recovered);
  //Serial.print("Recovered:");
  //Serial.print(Recovered); 


  // deaths
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 65);
  //M5.Lcd.printf("Deaths: %02d ",deaths);
  M5.Lcd.print("Deaths: " + deaths);
  //Serial.print("Deaths:");
  //Serial.print(deaths); 
      
}

void check_country(String sCountry) {
  int err =0;
  int readcounter = 0;
  int read_value_step = 0;
  String s1 = "";
  String s2 = "";

 if (sCountry == "World") { err = http.get("/coronavirus/"); }
  else {err = http.get("/coronavirus/country/" + sCountry +"/"); }
  if (err == 0)
  {
    Serial.println("Requesting ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
    
      unsigned long timeoutStart = millis();
      char c;
      while ( (http.connected() || http.available()) &&
             (!http.endOfBodyReached()) &&
             ((millis() - timeoutStart) < NetworkTimeout) )
      {
          if (http.available())
          {
              c = http.read();
              s2 = s2 + c;
              if (readcounter < 300) {
                readcounter++;
              } else {
                readcounter = 0;
                String tempString = "";
                tempString.concat(s1);
                tempString.concat(s2);
                // check infected first 
                if (read_value_step == 0) {                               
                  int place = tempString.indexOf("Coronavirus Cases:");
                  if ((place != -1) && (place < 350)) { 
                    read_value_step = 1;
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("#aaa") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                    //s1.remove(s1.indexOf(","),1);  
                    //Serial.print("Coronavirus Cases: ");
                    //Serial.println(s1);
                    infected = s1; //.toInt();
                    //Serial.println(infected);
                  }
                  
                }
                // check deaths               
                if (read_value_step == 1) {
                  int place = tempString.indexOf("Deaths:");
                  if ((place != -1) && (place < 350)) { 
                    read_value_step = 2;
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("<span>") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                    //s1.remove(s1.indexOf(","),1);  
                    //Serial.print("Deaths: ");
                    //Serial.println(s1);
                    deaths = s1; //.toInt();
                  }
                }                
                // check recovered               
                if (read_value_step == 2) {
                  int place = tempString.indexOf("Recovered:");
                  if ((place != -1) && (place < 350)) {                   
                    s2 = tempString.substring(place + 15);
                    tempString = s2.substring(s2.indexOf("<span>") + 6);
                    s1 = tempString.substring(0, (tempString.indexOf("</")));
                    //s1.remove(s1.indexOf(","),1);  
                    //Serial.print("Recovered: ");
                    //Serial.println(s1);
                    recovered = s1; //.toInt();
                    draw_country_screen(sCountry);
                    http.stop();
                    return;
                  }
                }                
      
                s1 = s2;
                s2 = ""; 
              }              
              
              timeoutStart = millis();
          }
          else
          {
              delay(NetworkDelay);
          }
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    //Serial.print("Connect failed: ");
    //Serial.println(err);
  }
  http.stop();
  
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  //Serial.print("SSID: ");
  //Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  //Serial.print("IP Address: ");
  //Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  //Serial.print("signal strength (RSSI):");
  //Serial.print(rssi);
  //Serial.println(" dBm");
}
void battery_status()
{
  vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  discharge = M5.Axp.GetIdischargeData() / 2;
  if (vbat >= 4)
  {
    M5.Lcd.pushImage(145, 1, 14, 8, bat_3);
  }
  else if (vbat >= 3.7)
  {
    M5.Lcd.pushImage(145, 1, 14, 8, bat_2);
  }
  else if (vbat < 3.7)
  {
    M5.Lcd.pushImage(145, 1, 14, 8, bat_1);
  }
  else
  {}
  // M5.Lcd.setTextColor(TFT_YELLOW);
  // M5.Lcd.setCursor(140, 12);
  // M5.Lcd.setTextSize(1);
  // M5.Lcd.println(discharge);
}
