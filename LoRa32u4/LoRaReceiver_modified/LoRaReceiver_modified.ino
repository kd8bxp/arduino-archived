#include <SPI.h>
#include <LoRa.h>
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

  Serial.println("LoRa Receiver");
LoRa.setPins(pinNSS, pinNRST, pinDIO0);
  if (!LoRa.begin(432300000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(loraSpreadingFactor);
  //LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
