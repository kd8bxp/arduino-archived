/*
 *  WeMos D1          RFM9x Module
  GPIO12 (D6) <----> MISO
  GPIO13 (D7) <----> MOSI
  GPIO14 (D5) <----> CLK
  GPIO15 (D8) <----> DIO0/D2 OR DIO1/D3 OR DIO2/D4
  GPIO02 (D4) <----> SEL (Chip Select)
  RST         <----> RST
  * Appears D1, D2, D3, A0, and D0 free
  */
  
#include <SPI.h>
#include "src/LoRa/LoRa.h"
#define SS      D4 
#define RST     22 //14 //22 
#define DI0     D8 

int       loraSpreadingFactor = 12;
int       loraPreamble      = 12;
int       loraTxPower         = 17;
uint32_t  loraBandwidth       = 125E6;
int       loraCodingRate  =  5;
int       loraSyncWord = 15;
int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  SPI.begin();
LoRa.setPins(SS,RST,DI0);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1) {delay(100);};
  }
  LoRa.disableCrc(); //encable CRC checking - off by default
  LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setPreambleLength(loraPreamble);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
  LoRa.setSyncWord(loraSyncWord);
  
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

  delay(1000);
}
