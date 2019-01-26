// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait serial port initialization
  }

  // Memory pool for JSON object tree.
  //
  // Inside the brackets, 200 is the size of the pool in bytes,
  // If the JSON object is more complex, you need to increase that value.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  StaticJsonBuffer<512> jsonBuffer;

  // StaticJsonBuffer allocates memory on the stack, it can be
  // replaced by DynamicJsonBuffer which allocates in the heap.
  //
  // DynamicJsonBuffer  jsonBuffer(200);

  // JSON input string.
  //
  // It's better to use a char[] as shown here.
  // If you use a const char* or a String, ArduinoJson will
  // have to make a copy of the input in the JsonBuffer.
  char json[] =  "{  \"TO\": \"CQ\", \"FROM\": \"KD8BXP-10\", \"MESSAGE\": \"This is a test message. We will make this message really long, longer than it should be because there should be a limit on message size. But right now there isn't a limit.\",  \"RT\": [    { \"CALL\": \"KD8BXP0\", \"RSSI\": -50  }, { \"CALL\": \"KD8BXP1\",\"RSSI\": -51 },{\"CALL\": \"KD8BXP2\",\"RSSI\": -52 },{\"CALL\": \"KD8BXP3\",\"RSSI\": -53 },{\"CALL\": \"KD8BXP4\",\"RSSI\": -54 }],\"CFG\": [{\"RTCOUNT\": 5,      \"GRID\": \"EM79\", \"TYPE\": \"BEACON\", \"OTHER\": \"\" }], \"TIME\": \"1900z\", \"DATE\": \"080917\"}";



  
  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(json);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  const char*  to = root["TO"];
  const char* from = root["FROM"];
  int rtcount = root["RTCOUNT"];
 // const char* jsonRT = root["RT"][0];
 // const char* jsonRT1 = root["RT"][1];
  const char* RTCALL0 = root["RT"][0]["CALL"];
  int rssi0 = root["RT"][0]["RSSI"];
  const char* grid = root["CFG"][0]["GRID"];

  Serial.print("TO: ");
  Serial.println(to);
  Serial.print("FROM: ");
  Serial.println(from);
  Serial.print("RTCount: ");
  Serial.println(rtcount);
  //Serial.print("RT0: ");
 // Serial.println(jsonRT);
  //Serial.print("RT1: ");
  //Serial.println(jsonRT1);
  Serial.print("RT Call 0: ");
  Serial.println(RTCALL0);
  Serial.print("RT RSSI 0: ");
  Serial.println(rssi0);
  Serial.print("Grid: ");
  Serial.println(grid);
}

void loop() {
  // not used in this example
}
