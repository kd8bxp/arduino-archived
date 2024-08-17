



#include "src/M5StickC/M5StickC.h"
#include "src/ArduinoJson5/ArduinoJson.h"  //use version 5.13.5

String json = "{\"x\":92, \"y\":35, \"w\":36, \"h\":42, \"id\":9, \"family\":16, \"cx\":109, \"cy\":57, \"rotation\":4.731833, \"decision_margin\":0.194354, \"hamming\":0, \"goodness\":0.000000, \"x_translation\":1.594086, \"y_translation\":0.138452, \"z_translation\":-6.166679, \"x_rotation\":3.534139, \"y_rotation\":6.255209, \"z_rotation\":4.731833}";


void setup() {
  
  Serial.begin (115200);
  
  M5.begin();
delay(1000);
  
StaticJsonBuffer<650> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(json);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

int x = root["x"];
int y = root["y"];
int w = root["w"];
int h = root["h"];
int id = root["id"];

Serial.print("x: "); Serial.println(x);
Serial.print("y: "); Serial.println(y);
Serial.print("w: "); Serial.println(w);
Serial.print("h: "); Serial.println(h);
Serial.print("ID: "); Serial.println(id);
 
}

void loop() {
 
}
