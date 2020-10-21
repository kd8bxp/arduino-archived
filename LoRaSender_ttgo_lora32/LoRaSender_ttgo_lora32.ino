#include <SPI.h>
#include <LoRa.h>

//These must match for your specific board, these should work for the LoRa32u4 boards, but if it fails, check here.
#define SS      18    
#define RST     14
#define DI0     26
#define BAND    432300000 // local frequencys, must match!
//For this to work on a local level these parameters need to match
int       loraSpreadingFactor = 9;
int       loraCodingRate      = 5;
int       loraTxPower         = 17;
uint32_t  loraBandwidth       = 62500; //125E6;
int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
  SPI.begin(5,19,27,18);
LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc(); //encable CRC checking - off by default
  LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
