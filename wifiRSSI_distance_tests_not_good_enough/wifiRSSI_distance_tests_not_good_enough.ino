/*
 * http://tdmts.net/2017/02/04/using-wifi-rssi-to-estimate-distance-to-an-access-point/
 * https://stackoverflow.com/questions/11217674/how-to-calculate-distance-from-wifi-router-using-signal-strength
 * https://www.digitalairwireless.com/articles/blog/wifi-transmit-power-calculations-made-simples
 * https://forums.estimote.com/t/determine-accurate-distance-of-signal/2858/4
 * https://github.com/tttapa/Projects/tree/master/ESP8266/WiFi/RSSI-WiFi-Quality
 */

 /*The formula to calculate the distance is pretty straightforward.

d = 10 ^ ((TxPower – RSSI) / (10 * n))

TxPower is the RSSI measured at 1m from a known AP. For example: -84 dB.
n is the propagation constant or path-loss exponent. For example: 2.7 to 4.3 (Free space has n = 2 for reference).
RSSI is the measured RSSI
d is the distance in meter
Be aware that this is a very basic attempt and should perhaps be considered more of a ‘proof of concept’ as to see how well you can estimate distance.
*/

#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
 #include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

#define WIFI_SSID "homeinternet"
#define WIFI_PASSWORD "trixie4me"

//TxPower is RSSI at 1 meter (3 feet)
//distance in meters
#define TxPower -30 //100mW 20dBm
SSD1306Wire  display(0x3c, D2, D1);
float count;

void setup() {
  Serial.begin(9600);
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.display();
  Serial.println("\r\n");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Try to connect to a given WiFi network
}

void loop() {
  printWiFiState();
display.clear();
  static int previousQuality = -1;
  int quality = getQuality();
  if (quality != previousQuality) {  // If the quality changed since last print, print new quality and RSSI
    if (quality != -1) {
      display.drawString(35,35,"RSSI: ");
      char buffer[5];
      sprintf(buffer,"%d",WiFi.RSSI());
      display.drawString(35,45,buffer);
      for (int i=0; i<100; i++) {
    int rs = WiFi.RSSI();
      
      display.drawString(35,15,"Distance: ");
      //Serial.print(pow(10,((TxPower-/*(int)WiFi.RSSI()*/rs)/20)));
      //Serial.print(" cm(?) ");
      //Serial.print(pow(10,((TxPower-/*(int)WiFi.RSSI()*/rs)/20))/30.48);
      float temp = pow(10,(((double)TxPower-rs)/(10 * 4.3))); // /30.48;
      count = count + temp;
      }
      Serial.printf("WiFi Quality:\t%d\%\tRSSI:\t%d dBm\tDistance: ", quality, WiFi.RSSI());
      Serial.print(count/99); Serial.print(" cm(?) ");
      Serial.print((count/99)*3.28); Serial.println(" ft(?)");
      //char buffer[5];
      dtostrf((count/99)*3.28, 4,2, buffer);
      //sprintf(buffer,"%s",temp.s_str());
      display.drawString(35,25,buffer);
     // Serial.println(" ft(?)");
    previousQuality = quality;
    display.display();
  }
  count = 0;
     
}

delay(1000);

}
/*
   If WiFi is connected, print the SSID and the IP address.
   If the WiFi is disconnected, print that it's disconnected.
*/
void printWiFiState() {
  static boolean printed = false;
  if (WiFi.status() == WL_CONNECTED) {
    if (printed)
      return;
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
    Serial.println();
    printed = true;
  } else {  // if WiFi is not connected
    if (!printed)
      return;
    Serial.println("WiFi disconnected!\n");
    printed = false;
  }
}

/*
   Return the quality (Received Signal Strength Indicator)
   of the WiFi network.
   Returns a number between 0 and 100 if WiFi is connected.
   Returns -1 if WiFi is disconnected.
*/
int getQuality() {
  if (WiFi.status() != WL_CONNECTED)
    return -1;
  int dBm = WiFi.RSSI();
  if (dBm <= -100)
    return 0;
  if (dBm >= -50)
    return 100;
  return 2 * (dBm + 100);
}
