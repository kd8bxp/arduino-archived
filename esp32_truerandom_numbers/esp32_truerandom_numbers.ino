#include "TridentTD_ESP_TrueRandom.h"

void setup() {
  Serial.begin(115200); Serial.println();

  Serial.println( esp.random());
  Serial.println( esp.random(1000));
  Serial.println( esp.random(20,50));
  Serial.println( esp.genUUID());
  Serial.println( esp.genMacAddress());
  Serial.println("------------------------");
}

void loop() {
  Serial.println( esp.random(1,7));
  delay(50);
}
