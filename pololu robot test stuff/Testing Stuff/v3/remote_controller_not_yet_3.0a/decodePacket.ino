void decodePacket(char* msg1) {
  StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(msg1);
Serial.println(msg1);
if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
      }

RobotID = root["RobotID"].asString();
m1dir = root["direction"][0];
m2dir = root["direction"][1];
m1steps = root["encoder"][0];
m2steps = root["encoder"][1];
distanceIN = root["distance"];
displayPkg();
}

void decodeHomePacket(char* msg1) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg1);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  IPremote = root["IP"].asString();
  subclientidTemp = root["robotID"].asString();
}

