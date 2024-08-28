#include "src/ArduinoJson5/ArduinoJson.h"
#include <Ethernet.h>
#include <SPI.h>

String h = "";
String d1 = "";
String d2 = "";
String d3 = "";
String d4 = "";
String d5 = "";
WiFiClient client;

// Name address for Open Weather Map API
const char* server = "api.openweathermap.org";
// Replace with your unique URL resource
const char* resource = "token";
// How your resource variable should look like, but with your own COUNTRY CODE, CITY and API KEY (that API KEY below is just an example):
//const char* resource = "/data/2.5/weather?q=Porto,pt&appid=bd939aa3d23ff33d3c8f5dd1";

const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// The type of data that we want to extract from the page
struct clientData {
  String mainWeather[8];
  String description[8];
  String tempMax[8];
  String tempMin[8];
  String location;
  String currentTime[8];
  String icon[8];

};

// ARDUINO entry point #2: runs over and over again forever
void wiFiConnect() {
  if (connect(server)) {
    if (sendRequest(server, resource) && skipResponseHeaders()) {
      clientData clientData;
      m5.Lcd.println("All Valid Requirement Are Present");
      //delay(5000);
      if (readReponseContent(&clientData)) {
        printclientData(&clientData);
      }
    }
    else {
      m5.Lcd.println("Not All Valid Requirement Are Present");
    }
  }
  disconnect();
  //wait();
}

// Open connection to the HTTP server
bool connect(const char* hostName) {
  m5.Lcd.print("Connect to ");
  m5.Lcd.println(hostName);

  bool ok = client.connect(hostName, 80);

  m5.Lcd.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* resource) {
  m5.Lcd.print("GET ");
  m5.Lcd.println(resource);

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  m5.Lcd.println("SendRequest COMPLETE");
  return true;
}

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    m5.Lcd.println("No response or invalid response!");
  }
  else {
    m5.Lcd.println("SkipResponseHeaders COMPLETE");
  }
  return ok;
}


bool readReponseContent(struct clientData* clientData) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  const size_t bufferSize = 2 * JSON_ARRAY_SIZE(0) + 9 * JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(8) + JSON_OBJECT_SIZE(1) + 17 * JSON_OBJECT_SIZE(4) + 8 * JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(14) + 8 * JSON_OBJECT_SIZE(15);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.parseObject(client);

  if (!root.success()) {
    m5.Lcd.println("JSON parsing failed!");
    return false;
  }
  else {
    m5.Lcd.println("ReadReponseContent COMPLETE");
  }

  clientData->mainWeather[0] = root["current"]["weather"][0]["main"].as<String>();
  clientData->description[0] = root["current"]["weather"][0]["description"].as<String>();
  clientData->tempMax[0] = root["current"]["temp"].as<String>();
  clientData->tempMin[0] = root["current"]["temp"].as<String>();
  clientData->icon[0] = root["current"]["weather"][0]["icon"].as<String>();
  clientData->currentTime[0] = timeConvert(root["current"]["dt"].as<int>() + root["timezone_offset"].as<int>());

  // Here were copy the strings we're interested in using to your struct data
  for (int i = 1; i < 8 ; i++) {
    clientData->mainWeather[i] = root["daily"][i]["weather"][0]["main"].as<String>();
    clientData->description[i] = root["daily"][i]["weather"][0]["description"].as<String>();
    clientData->tempMax[i] = root["daily"][i]["temp"]["max"].as<String>();
    clientData->tempMin[i] = root["daily"][i]["temp"]["min"].as<String>();
    clientData->icon[i] = root["daily"][i]["weather"][0]["icon"].as<String>();
    clientData->currentTime[i] = timeConvert(root["daily"][i]["dt"].as<int>() + root["timezone_offset"].as<int>());
  }

  clientData->location = root["timezone"].as<String>();

  imagesForecast.imgBackground(TFT_BLACK);
  imagesForecast.imgFromTop(20);
  imagesForecast.imgCaptionColor(TFT_WHITE);
  imagesForecast.imgCaptionFont(&FreeSansBold9pt7b);
  imagesForecast.imgCaptionLocation(TC_DATUM);
  imagesForecast.imgCaptionColor(TFT_WHITE);


  for (int i = 0; i < 5  ; i++) {
    String s = clientData->currentTime[i];



    if (clientData->icon[i] == "01d") {
      if (i == 0) {
        imagesForecast.addItem(Pic01d, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic01d, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic01d, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic01d, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic01d, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "01n") {
      if (i == 0) {
        imagesForecast.addItem(Pic01n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic01n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic01n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic01n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic01n, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "02d") {
      if (i == 0) {
        imagesForecast.addItem(Pic02d, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic02d, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic02d, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic02d, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic02d, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "02n" || clientData->icon[i] == "03d" || clientData->icon[i] == "03n") {
      if (i == 0) {
        imagesForecast.addItem(Pic02n03d03n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic02n03d03n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic02n03d03n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic02n03d03n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic02n03d03n, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "04d" || clientData->icon[i] == "04n") {
      if (i == 0) {
        imagesForecast.addItem(Pic04d04n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic04d04n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic04d04n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic04d04n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic04d04n, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "09d") {
      if (i == 0) {
        imagesForecast.addItem(Pic09d, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic09d, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic09d, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic09d, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic09d, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "09n") {
      if (i == 0) {
        imagesForecast.addItem(Pic09n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic09n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic09n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic09n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic09n, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "10d") {
      if (i == 0) {
        imagesForecast.addItem(Pic10d, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic10d, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic10d, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic10d, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic10d, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "10n") {
      if (i == 0) {
        imagesForecast.addItem(Pic10n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic10n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic10n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic10n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic10n, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "11d" || clientData->icon[i] == "11n") {
      if (i == 0) {
        imagesForecast.addItem(Pic11d11n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic11d11n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic11d11n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic11d11n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic11d11n, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "13d") {
      if (i == 0) {
        imagesForecast.addItem(Pic13d, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic13d, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic13d, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic13d, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic13d, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "13n") {
      if (i == 0) {
        imagesForecast.addItem(Pic13n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic13n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic13n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic13n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic13n, s, mainmenu_msgs5);
      }
    }
    else if (clientData->icon[i] == "50d" || clientData->icon[i] == "50n") {
      if (i == 0) {
        imagesForecast.addItem(Pic50d50n, s, mainmenu_msgs1);
      }
      else if (i == 1) {
        imagesForecast.addItem(Pic50d50n, s, mainmenu_msgs2);
      }
      else if (i == 2) {
        imagesForecast.addItem(Pic50d50n, s, mainmenu_msgs3);
      }
      else if (i == 3) {
        imagesForecast.addItem(Pic50d50n, s, mainmenu_msgs4);
      }
      else if (i == 4) {
        imagesForecast.addItem(Pic50d50n, s, mainmenu_msgs5);
      }
    }
  }
  imagesForecast.addItem(system_jpg, "Settings", ez.settings.menu);    // This image comes from images.h file
  imagesForecast.addItem(sleep_jpg, "Power Off", powerOff);

  h = "Information";
  d1 = clientData->location + "|" + clientData->currentTime[0] + "|Weather : " + clientData->mainWeather[0] + "|Description : "
       + clientData->description[0] + "|Max Tem :" + clientData->tempMax[0] +
       "|Min Tem :" + clientData->tempMin[0];
  d2 = clientData->location + "|" + clientData->currentTime[1] + "|Weather : " + clientData->mainWeather[1] + "|Description : "
       + clientData->description[1] + "|Max Tem :" + clientData->tempMax[1] +
       "|Min Tem :" + clientData->tempMin[1];
  d3 = clientData->location + "|" + clientData->currentTime[2] + "|Weather : " + clientData->mainWeather[2] + "|Description : "
       + clientData->description[2] + "|Max Tem :" + clientData->tempMax[2] +
       "|Min Tem :" + clientData->tempMin[2];
  d4 = clientData->location + "|" + clientData->currentTime[3] + "|Weather : " + clientData->mainWeather[3] + "|Description : "
       + clientData->description[3] + "|Max Tem :" + clientData->tempMax[3] +
       "|Min Tem :" + clientData->tempMin[3];
  d5 = clientData->location + "|" + clientData->currentTime[4] + "|Weather : " + clientData->mainWeather[4] + "|Description : "
       + clientData->description[4] + "|Max Tem :" + clientData->tempMax[4] +
       "|Min Tem :" + clientData->tempMin[4];




  // It's not mandatory to make a copy, you could just use the pointers
  // Since, they are pointing inside the "content" buffer, so you need to make
  // sure it's still in memory when you read the string

  return true;
}

void mainmenu_msgs1() {
  ez.msgBox(h, d1, "OK", true, &FreeSansBold9pt7b);
}
void mainmenu_msgs2() {
  ez.msgBox(h, d2, "OK", true, &FreeSansBold9pt7b);
}
void mainmenu_msgs3() {
  ez.msgBox(h, d3, "OK", true, &FreeSansBold9pt7b);
}
void mainmenu_msgs4() {
  ez.msgBox(h, d4, "OK", true, &FreeSansBold9pt7b);
}
void mainmenu_msgs5() {
  ez.msgBox(h, d5, "OK", true, &FreeSansBold9pt7b);
}


// Print the data extracted from the JSON
void printclientData(const struct clientData* clientData) {
  for (int i = 0; i < 7 ; i++) {
    m5.Lcd.print("currentTime = ");
    m5.Lcd.println(clientData->currentTime[i]);
    m5.Lcd.print("location = ");
    m5.Lcd.println(clientData->location);
    m5.Lcd.print("mainWeather = ");
    m5.Lcd.println(clientData->mainWeather[i]);
    m5.Lcd.print("description = ");
    m5.Lcd.println(clientData->description[i]);
    m5.Lcd.print("tempMax = ");
    m5.Lcd.println(clientData->tempMax[i]);
    m5.Lcd.print("tempMin = ");
    m5.Lcd.println(clientData->tempMin[i]);
    m5.Lcd.print("");
      }
      
}


// Close the connection with the HTTP server
void disconnect() {
  m5.Lcd.println("Disconnect");
  client.stop();
}

// Pause for a 1 minute
void wait() {
  m5.Lcd.println("Wait 60 seconds");
  delay(60000);
}
