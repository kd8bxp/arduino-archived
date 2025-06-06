
#include "src/esp8266_weather_station/ESPWiFi.h"
#include "src/esp8266_weather_station/ESPHTTPClient.h"
#include "src/json-streaming-parser/JsonListener.h"

// time

#include "src/esp8266_ssd1306/SSD1306.h"
#include "src/esp8266_ssd1306/OLEDDisplayUi.h"
#include "Wire.h"
#include "src/esp8266_weather_station/OpenWeatherMapCurrent.h"
#include "src/esp8266_weather_station/OpenWeatherMapForecast.h"
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"

#include "ESP8266WiFi.h"
//#include <MQTT.h>
//#include "WiFiUDP.h"

#include <Ticker.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h> 

#define TZ              -5       // (utc+) TZ in hours
#define DST_MN          60
#define TZ_MN           ((TZ)*60)
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)

/***************************
 * Begin Settings
 **************************/
int onoff = 0;
int onoff2 = 0;
int swithmode = 0;
int buttons[3] = {12, 13, 14};
int interrupt = 0;

time_t now;

// WIFI
const char* WIFI_SSID = "";
const char* WIFI_PWD = "";

// MQTT info
//const char* hostname = "broker.hivemq.com"; //"m16.cloudmqtt.com";
//int port = 8000;
//const char* user = ""; //"fvamicuw";
//const char* user_password = ""; //"ZvzVfvn_OmUV";
//const char* id = "watch636452534234325435";

// Map ID & LOCATION ID
String OPEN_WEATHER_MAP_APP_ID = "";
//String OPEN_WEATHER_MAP_LOCATION_ID = "4509884"; //dayton, ohio
String OPEN_WEATHER_MAP_LOCATION_ID = "4518264"; //middletown, ohio


WiFiClient net;
//MQTTClient client;
//WiFiUDP udp;
Ticker ticker;

const int UPDATE_INTERVAL_SECS = 20 * 60; // Update every 20 minutes

// Chinese Simplified - zh_cn
String OPEN_WEATHER_MAP_LANGUAGE = "en";
const uint8_t MAX_FORECASTS = 4;

const boolean IS_METRIC = false;

// Adjust according to your language
const String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};


 //SH1106Wire  display(0x3C, 5, 4);
 SSD1306 display(0x3c, D5, D6);
 OLEDDisplayUi   ui( &display );

OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapCurrent currentWeatherClient;

OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
OpenWeatherMapForecast forecastClient;


// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = false;

String lastUpdate = "--";

long timeSinceLastWUpdate = 0;

//declaring prototypes
void drawProgress(OLEDDisplay *display, int percentage, String label);
void updateData(OLEDDisplay *display);
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);
void setReadyForWeatherUpdate();


// Add frames
// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
FrameCallback frames[] = { drawDateTime, drawCurrentWeather, drawForecast  };
//FrameCallback frames[] = {drawCurrentWeather, drawForecast };
int numberOfFrames = 3;
//int numberOfFrames = 2;



void setup() {

  for (int i=0; i<3; i++) {
    pinMode(buttons[i], INPUT);
  }
  
  Serial.begin(9600);

   // Get time from network time service
  configTime(TZ_SEC, DST_SEC, "pool.ntp.org");

  // initialize dispaly
  display.init();
  display.clear();
  display.display();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);

  WiFi.begin(WIFI_SSID, WIFI_PWD);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawString(64, 10, "Connecting to WiFi");
    display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbole : inactiveSymbole);
    display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbole : inactiveSymbole);
    display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbole : inactiveSymbole);
    display.display();

    counter++;
  }
  
 // client.begin(hostname, net);
 // client.onMessage(messageReceived);
//client.subscribe("/relay");

  ui.setTargetFPS(30);

  ui.setActiveSymbol(activeSymbole);
  ui.setInactiveSymbol(inactiveSymbole);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  ui.setFrames(frames, numberOfFrames);

  // Inital UI takes care of initalising the display too.
  ui.init();

  Serial.println("");

  updateData(&display);

}

void loop() {
  //client.loop();

  //if (!client.connected()) {
  //  connect();
 // }
  if (millis() - timeSinceLastWUpdate > (1000L*UPDATE_INTERVAL_SECS)) {
    setReadyForWeatherUpdate();
    timeSinceLastWUpdate = millis();
  }
//showtime();
//delay(1000);
showforcast();
//delay(1000);

/* switch(swithmode){
  case 0 :
  {showtime();}
   break;

   case 1:
   {showforcast();}
   break;

   case 2:
   {show1();}
   break;

    case 3 :
     {show2();}
   break;
}

if(digitalRead(buttons[0]) == LOW) {
if (swithmode < 3)
    {
      swithmode = swithmode + 1;
    }else{
      swithmode = 0;
    }

    Serial.print((String)swithmode);

delay(500);
}
*/

/*if(digitalRead(buttons[1]) == LOW) {

   if (swithmode == 2)
  {
   if(onoff == 0)
  {
    client.publish("/relay", "on1", 0, 1);
    onoff =1;
 }else{
  client.publish("/relay", "off1", 0, 1);
  onoff = 0;
   }
   show1();
   }
   
   if(swithmode == 3)
   {
   if(onoff2 == 0)
   {
    client.publish("/relay", "on2", 0, 1);
    onoff2 = 1;
 }else{
  client.publish("/relay", "off2", 0, 1);
  onoff2 = 0;
   }
   show2();
 }
 delay(500);
 }*/
}

void showforcast()
{
   int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    delay(remainingTimeBudget);
  }
}


void drawProgress(OLEDDisplay *display, int percentage, String label) {
  display->clear();
  display->flipScreenVertically();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}

void updateData(OLEDDisplay *display) {
  drawProgress(display, 10, "Updating time...");
  drawProgress(display, 30, "Updating weather...");
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrentById(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
  drawProgress(display, 50, "Updating forecasts...");
  forecastClient.setMetric(IS_METRIC);
  forecastClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  uint8_t allowedHours[] = {12};
  forecastClient.setAllowedHours(allowedHours, sizeof(allowedHours));
  forecastClient.updateForecastsById(forecasts, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID, MAX_FORECASTS);

  readyForWeatherUpdate = false;
  drawProgress(display, 100, "Done...");
  delay(1000);
}



void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
 //display->clear();
 //display->setTextAlignment(TEXT_ALIGN_LEFT);
   now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[16];

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(30,0,"");
  
  String date = WDAY_NAMES[timeInfo->tm_wday];
  
  display->setFont(ArialMT_Plain_24);
  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display->drawString(60 + x, 5 + y, String(buff));
  sprintf_P(buff, PSTR("%02d/%02d/%04d"),timeInfo->tm_mon+1, timeInfo->tm_mday, timeInfo->tm_year + 1900);
  display->drawString(60 + x, 30 + y, String(buff));

  
  display->setTextAlignment(TEXT_ALIGN_LEFT);
 //display.display();
  //display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, currentWeather.description);

  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWeather.temp, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 5 + y, temp);

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(32 + x, 0 + y, currentWeather.iconMeteoCon);
}


void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawForecastDetails(display, x, y, 0);
  drawForecastDetails(display, x + 44, y, 1);
  drawForecastDetails(display, x + 88, y, 2);
}

void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex) {
  time_t observationTimestamp = forecasts[dayIndex].observationTime;
  struct tm* timeInfo;
  timeInfo = localtime(&observationTimestamp);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y, WDAY_NAMES[timeInfo->tm_wday]);

  display->setFont(Meteocons_Plain_21);
  display->drawString(x + 20, y + 12, forecasts[dayIndex].iconMeteoCon);
  String temp = String(forecasts[dayIndex].temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y + 34, temp);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void setReadyForWeatherUpdate() {
  Serial.println("Setting readyForUpdate to true");
  readyForWeatherUpdate = true;
}


/*void showtime()
{
  display.clear();
   now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[16];

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(30,0,"");
  
  String date = WDAY_NAMES[timeInfo->tm_wday];
  
  display.setFont(ArialMT_Plain_24);
  sprintf_P(buff, PSTR("%02d/%02d/%04d"),timeInfo->tm_mon+1, timeInfo->tm_mday, timeInfo->tm_year + 1900);
  display.drawString(60,15, String(buff));

  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display.drawString(60, 40, String(buff));
  display.setTextAlignment(TEXT_ALIGN_LEFT);
 display.display();
}

void show1()
 {
 display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(10,0,"Makerfabs"); 
    display.setFont(ArialMT_Plain_24);
    display.drawString(20,24, "Light:");
    if(onoff == 1){
    display.drawString(86,24,"on");
    }else{
       display.drawString(86,24,"off");
    }
    display.display();
 }

 void show2()
 {
 display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(10,0,"Makerfabs"); 
    display.setFont(ArialMT_Plain_24);
    display.drawString(20,24, "Fan:");
    if(onoff2 == 1){
    display.drawString(86,24,"on");
    }else{
       display.drawString(86,24,"off");
    }
    display.display();
 }
*/
 
//void messageReceived(String &topic, String &payload) {
//  Serial.println("incoming: " + topic + " - " + payload);
//}

void connect() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  //Serial.println("connecting to MQTT...");
 // if (!client.connect(id, user, user_password)) {
 //   return;
  //}

  Serial.println("\nconnected!");

  //client.subscribe("/hello");
  // client.unsubscribe("/hello");
}
