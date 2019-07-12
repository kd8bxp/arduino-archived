#include "ESP8266WiFi.h"
#include "SSD1306.h"
#include "DHT.h"

extern "C" {
  #include <espnow.h>
}

#define OLED_SDA   D2 // D1  // SDA-PIN for I2C OLED
#define OLED_SCL   D1 // D2  // SCL-PIN for I2C OLED
#define DHTPIN D4
#define NODE 1 //number of the ESP-Now node
#define WIFI_CHANNEL 1

// Uncomment whatever type you're using!
//#define DHTTYPE DHT12   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
SSD1306 display(0x3c, OLED_SDA, OLED_SCL); // FOR I2C

float h,f, hif; //Humidity and Fahrenheit, heat index

// this is the MAC Address of the remote ESP which this ESP sends its data too
uint8_t remoteMac[] = {0x2C, 0x3A, 0xE8, 0x0E, 0xD8, 0x9E};

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA); // Station mode for sensor/controller node
  WiFi.begin();
  Serial.print("This node mac: "); Serial.println(WiFi.macAddress());
display.init();
display.flipScreenVertically();
display.clear();
display.setFont(ArialMT_Plain_10);
display.drawString(10,36,"Hello");
display.display();

delay(100);
Serial.begin(9600);
delay(100);
Serial.flush();
delay(100);
dht.begin();

if (esp_now_init()!=0) {
    Serial.println("*** ESP_Now init failed");
    ESP.restart();
  }

  delay(1); // This delay seems to make it work more reliably???
esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(remoteMac, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);

esp_now_register_send_cb([](uint8_t* mac, uint8_t status) {
    Serial.print("send_cb, status = "); Serial.print(status); 
    Serial.print(", to mac: "); 
    char macString[50] = {0};
    sprintf(macString,"%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println(macString);
    
  });

}

void loop() {
  updateTemp();
  displayTemp();
  sendTemp();
  delay(1000);
  yield();
}

void updateTemp() {
  h=dht.readHumidity();
  f=dht.readTemperature(true);
  hif = dht.computeHeatIndex(f, h);
}

void displayTemp() {
  char buffer[25];
  char buffer2[25];
  char buffer3[25];
  sprintf(buffer,"H : %d.%02d%%",(int)h,(int)(h*100)%100);
  sprintf(buffer2,"T : %d.%02dF",(int)f,(int)(f*100)%100);
  sprintf(buffer3,"HI: %d.%02dF",(int)hif,(int)(hif*100)%100);
  display.clear();
  //display.setTextSize(2);
  display.drawString(32,20, buffer);
  display.drawString(32,29, buffer2);
  display.drawString(32,38, buffer3);
  display.display();
  yield();
}

void sendTemp() {
  char buffer[100];
  sprintf(buffer, "N: %d, H: %d.%02d, T: %d.%02dF, HI: %d.%02dF",NODE,(int)h,(int)(h*100)%100,(int)f,(int)(f*100)%100,(int)hif,(int)(hif*100)%100);
  esp_now_send(NULL, (unsigned char*)buffer,100);
}

