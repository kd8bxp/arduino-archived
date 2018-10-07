// Include required libraries
#include <SPI.h>
#include <string.h>
#include "DHT.h"
 
// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11
 
int tempRedLed = 14;
int tempBlueLed = 16;
int tempGreenLed = 15;

// DHT instance
DHT dht(DHTPIN, DHTTYPE);
                                         
void setup(void)
{
 pinMode(tempRedLed, OUTPUT);
 pinMode(tempBlueLed, OUTPUT);
 pinMode(tempGreenLed, OUTPUT);
 digitalWrite(tempRedLed, HIGH);
 digitalWrite(tempBlueLed, HIGH);
 digitalWrite(tempGreenLed, HIGH);
  // Initialize DHT sensor
  dht.begin();
  
 //   Serial.begin(115200);
  Serial.begin(9600);
}
 
void loop(void)
{
  // Wait a few seconds between measurements.
  delay(1000);
  
  // Measure the humidity & temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
 
 if (f >= 70.0 && f < 82.0) {
 	digitalWrite(tempRedLed, HIGH);
 	digitalWrite(tempBlueLed, HIGH);
 	digitalWrite(tempGreenLed, LOW);
 	 }
 
 if (f >=82.0) {
 	digitalWrite(tempRedLed, LOW);
 	digitalWrite(tempBlueLed, HIGH);
 	digitalWrite(tempGreenLed, HIGH);
 }
 
 if (f < 70.0) {
 	digitalWrite(tempRedLed, HIGH);
 	digitalWrite(tempBlueLed, LOW);
 	digitalWrite(tempGreenLed, HIGH);
 }
 
  /*
    // Transform to String
    String tempc = String((int) t);
    String tempF = String((int) f);
    String hum = String((int) h);
    String heat = String((int) hi); 
    
  Serial.print("Humidity: "); 
  Serial.print(hum);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(tempc);
  Serial.print(" *C , ");
  Serial.print(f);
  Serial.print(" *F\t  ");
  Serial.print("Heat index: ");
  Serial.print(heat);
  Serial.println(" *F");
   */
}

