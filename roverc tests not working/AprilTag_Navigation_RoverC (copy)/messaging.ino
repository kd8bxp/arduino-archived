#include "src/ArduinoJson5/ArduinoJson.h"

void decodeJson() {
StaticJsonBuffer<650> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(inputString);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

//tagInfo.x = root["x"];
//tagInfo.y = root["y"];
//tagInfo.w = root["w"];
//tagInfo.h = root["h"];
tagInfo.id = root["id"];
tagInfo.cx = root["cx"];
tagInfo.cy = root["cy"];
tagInfo.rotation = root["rotation"];

//Serial.print("x: "); Serial.println(x);
//Serial.print("y: "); Serial.println(y);
//Serial.print("w: "); Serial.println(w);
//Serial.print("h: "); Serial.println(h);
//Serial.print("ID: "); Serial.println(id);
 
}

void readSerialEvent() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    if (inChar == '\n') {
      stringComplete = true;
    } else {    inputString += inChar; }
    
  }
}
