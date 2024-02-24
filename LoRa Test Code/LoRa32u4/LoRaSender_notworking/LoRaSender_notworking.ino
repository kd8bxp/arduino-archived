#include <SPI.h>
#include <LoRa.h>

int counter = 0;
const int pinNSS  = 8;
    const int pinNRST = 4;
    const int pinDIO0 = 7;
     int       loraSpreadingFactor = 9;
  int       loraCodingRate      = 5;
  int       loraTxPower         = 17;
  uint32_t  loraBandwidth       = 62500; //125E6;
  uint32_t  loraFrequency       = 432300000; //434E6;
  
void setup() {
  Serial.begin(9600);
  while (!Serial);
LoRa.setPins(pinNSS, pinNRST, pinDIO0);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(432300000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
   LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("kd8bxp-2 kd8bxp ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
