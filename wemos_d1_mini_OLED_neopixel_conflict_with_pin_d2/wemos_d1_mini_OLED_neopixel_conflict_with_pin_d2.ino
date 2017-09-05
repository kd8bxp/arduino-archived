#include "ESP8266WiFi.h"
#include <ESP_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define OLED_RESET  16  // Pin 15 -RESET digital signal
#define OLED_RESET D0  // RST-PIN for OLED (not used)
#define OLED_SDA   D2 // D1  // SDA-PIN for I2C OLED
#define OLED_SCL   D1 // D2  // SCL-PIN for I2C OLED

ESP_SSD1306 display(OLED_RESET); // FOR I2C

String lastCommandString = "black";
int brightness = 15;
const char ssid[]     = "Motel6";
const char password[] = "";

const char host[] = "api.thingspeak.com";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, D2, NEO_GRB + NEO_KHZ800);


void setup() {
  
Wire.begin(OLED_SDA, OLED_SCL);
Wire.setClock(400000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Switch OLED
display.clearDisplay();
delay(100);
Serial.begin(9600);
delay(100);
Serial.flush();
delay(100);
strip.begin();
strip.show();
strip.setBrightness(brightness);
WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

 
getColor();

 
}

void loop() {
  badge();
  getColor();
}

void badge() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(8,0);
  display.clearDisplay();
  display.println("LeRoy");
  display.setCursor(15, 20);
  display.println("Miller");
  display.setCursor(25, 40);
  display.println("KD8BXP");
  display.display();
  scroll();
  delay(1000);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Automation");
  display.setCursor(0,20);
  display.println("Technology");
  display.setCursor(25,40);
  display.println("Club");
  display.display();
  scroll();
  delay(1000);
  display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(0,20);
  display.println("kd8bxp.blogspot.com");
  display.setCursor(0,30);
  display.println("github.com/kd8bxp");
  display.display();
  delay(2000);
  display.startscrollleft(0x00, 0x0F);
  delay(15000);
  display.stopscroll();
}

void scroll() {
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(4000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(4000);
  display.stopscroll();
}

void getColor() {

Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/channels/1417/field/1/last.txt";

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
            "Host: " + host + "\r\n" +
            "Connection: close\r\n\r\n");


// Process CheerLights Commands
delay(100);
String response;
char charIn;
do {
charIn = client.read(); // read a char from the buffer
response += charIn; // append that char to the string response
} while (client.available() > 0);
//Serial.println(response.length());
response.remove(519,7);
Serial.println(response);

if (response.indexOf("white") > 0 || response.indexOf("warmwhite") > 0)
{
lastCommandString = "white";
colorWipe(strip.Color(255,255,255),50);
}
else if (response.indexOf("black") > 0 || response.indexOf("off") > 0)
{
lastCommandString = "black";
colorWipe(strip.Color(0,0,0),50);
}
else if (response.indexOf("red") > 0)
{
lastCommandString = "red";
colorWipe(strip.Color(255,0,0),50);
}
else if (response.indexOf("green") > 0)
{
lastCommandString = "green";
colorWipe(strip.Color(0, 255, 0), 50);
}
else if (response.indexOf("blue") > 0)
{
lastCommandString = "blue";
colorWipe(strip.Color(0,0,255),50);
}
else if (response.indexOf("cyan") > 0)
{
lastCommandString = "cyan";
colorWipe(strip.Color(0,255,255),50);
}
else if (response.indexOf("magenta") > 0)
{
lastCommandString = "magenta";
colorWipe(strip.Color(255,0,255),50);
}
else if (response.indexOf("yellow") > 0)
{
lastCommandString = "yellow";
colorWipe(strip.Color(255,255,0),50);
}
else if (response.indexOf("purple") > 0)
{
lastCommandString = "purple";
colorWipe(strip.Color(102,51,204),50);
}
else if (response.indexOf("orange") > 0)
{
lastCommandString = "orange";
colorWipe(strip.Color(255,153,0),50);
}

else
{
lastCommandString = "(no match)";
}
/*
Serial.print("CheerLight Command Received: ");
Serial.println(lastCommandString);
*/
}


void colorWipe(uint32_t c, uint8_t wait) {
  for (int t=0; t<250; t++) {
for(uint16_t i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, c);
strip.show();
strip.setPixelColor(i, 0,0,0);
delay(10); //delay(wait);

}
strip.setPixelColor(strip.numPixels(), 0,0,0);
strip.show();
  }
  
}



