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
#include <LoRa.h>
#define SS      D4 
#define RST     22 
#define DI0     D8 

//float t, h; //tempature and humidity
float t,h;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
 LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    //Serial.print("Received packet '");

String buffer;
    // read packet
    while (LoRa.available()) {
      //Serial.print((char)LoRa.read());
      buffer = buffer+(char)LoRa.read();
    }
    Serial.println(buffer);
int t1,t2,t3,t4;

//char* buffer1;
//buffer.toCharArray(buffer1,7);
//Serial.print(buffer1);

//sscanf((char*)buffer[buffer.length()], "%d.%02d %d.%02d", &t1,&t2,&t3,&t4);
//sscanf(buffer1,"%d %d", &t,&h);
t = buffer.toFloat();
buffer.remove(0,6);
h = buffer.toFloat();
//t = t1 + (float)t2/100;
//h = t3 + (float)t4/100;
    // print RSSI of packet
    Serial.print(" T: ");Serial.print(t); Serial.print(" H: ");Serial.println(h);
    yield();
  }
  yield();
}
