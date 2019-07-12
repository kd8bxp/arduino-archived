#include "ESP8266WiFi.h"
#include "SSD1306.h"
#include "DHT.h"

#define OLED_SDA   D2 // D1  // SDA-PIN for I2C OLED
#define OLED_SCL   D1 // D2  // SCL-PIN for I2C OLED
#define DHTPIN D4

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
SSD1306 display(0x3c, OLED_SDA, OLED_SCL); // FOR I2C

float h,f, hif; //Humidity and Fahrenheit, heat index

void setup() {
  
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
}

void loop() {
  updateTemp();
  displayTemp();
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

