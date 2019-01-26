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
  // Inside the brackets, 200 is the size of the pool in bytes.
  // If the JSON object is more complex, you need to increase that value.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  
  DynamicJsonBuffer jsonBuffer;
  //StaticJsonBuffer<512> jsonBuffer;
  //StaticJsonBuffer<255> rtBuffer;
  // StaticJsonBuffer allocates memory on the stack, it can be
  // replaced by DynamicJsonBuffer which allocates in the heap.
  //
  // DynamicJsonBuffer  jsonBuffer(200);

  // Create the root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.createObject();
  

root["TO"] = "CQ";
root["FROM"] = "KD8BXP-10";
root["MESSAGE"] = "This is a test message. We will make this message really long, longer than it should be because there should be a limit on message size. But right now there isn't a limit.";

//root["RTCOUNT"] = 5;
/*JsonArray& rt = root.createNestedArray("RT");
  JsonObject& DIGI = rt.createNestedObject();
  DIGI["CALL"] = "KD8BXP";
  DIGI["RSSI"] = -50;
  JsonObject& DIGI1 = rt.createNestedObject();
  DIGI1["CALL"] = "KD8BXP-10";
  DIGI1["RSSI"] = -30;
  JsonObject& DIGI2 = rt.createNestedObject();
  DIGI2["CALL"] = "MYCALL";
  DIGI2["RSSI"] = -60;
  JsonObject& DIGI3 = rt.createNestedObject();
  DIGI3["CALL"] = "1MYCALL";
  DIGI3["RSSI"] = -160;
  JsonObject& DIGI4 = rt.createNestedObject();
  DIGI4["CALL"] = "M6YCALL";
  DIGI4["RSSI"] = -650;
  */
   JsonArray& rt = root.createNestedArray("RT");
  //JsonObject& RTID = rtBuffer.createObject();
  //JsonObject& RTID = rt.createNestedObject();
 //rt.add("RTCOUNT = 5");
  for(int i=0; i<5; i++) {
   JsonObject& RTID = rt.createNestedObject();
    RTID["CALL"] = "KD8BXP" + (String)i;
    RTID["RSSI"] = -50 - i;
    RTID["DEVICEID"] = "";
    RTID["DEVICETYPE"] = "GATEWAY";
    RTID["RTTIME"] = "";
    RTID["RTDATE"] = "";
  }
//JsonObject& rtcnt = rt.createNestedObject();
//rtcnt["RTCOUNT"] = 5;
   
   JsonArray& cfg = root.createNestedArray("CFG");
   JsonObject& cfgbuf = cfg.createNestedObject();
   cfgbuf["RTCOUNT"] = 5;
   cfgbuf["GRID"] = "EM79";
   cfgbuf["TYPE"] = "BEACON";
   cfgbuf["OTHER"] = "";
   cfgbuf["DEVICEID"] = "";
   
   root["TIME"] = "1900z";
   root["DATE"] = "080917";
   
  root.printTo(Serial);
  // This prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  Serial.println();

  root.prettyPrintTo(Serial);
  // This prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }
}

void loop() {
  // not used in this example
}
