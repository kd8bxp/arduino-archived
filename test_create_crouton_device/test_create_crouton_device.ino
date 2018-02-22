//http://crouton.mybluemix.net/crouton/dashboard
//http://crouton.mybluemix.net/documentation#how-it-works-connecting-to-crouton
//https://github.com/edfungus/Crouton

#include <ArduinoJson.h>


void setup() {
  Serial.begin(9600);
  const size_t bufferSize = 5*JSON_OBJECT_SIZE(1) + 6*JSON_OBJECT_SIZE(4);
DynamicJsonBuffer jsonBuffer(bufferSize);

JsonObject& root = jsonBuffer.createObject();

JsonObject& deviceInfo = root.createNestedObject("deviceInfo");
deviceInfo["name"] = "Temperatures";

JsonObject& deviceInfo_endPoints = deviceInfo.createNestedObject("endPoints");

JsonObject& deviceInfo_endPoints_barDoor = deviceInfo_endPoints.createNestedObject("Node1");
deviceInfo_endPoints_barDoor["title"] = "Node 1";
deviceInfo_endPoints_barDoor["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_barDoor["units"] = "F";
JsonObject& deviceInfo_endPoints_barDoor_values = deviceInfo_endPoints_barDoor.createNestedObject("values");
deviceInfo_endPoints_barDoor_values["value"] = 0;

JsonObject& deviceInfo_endPoints_door2 = deviceInfo_endPoints.createNestedObject("Node2");
deviceInfo_endPoints_door2["title"] = "Node 2";
deviceInfo_endPoints_door2["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_door2["units"] = "F";
JsonObject& deviceInfo_endPoints_door2_values = deviceInfo_endPoints_door2.createNestedObject("values");
deviceInfo_endPoints_door2_values["value"] = 0;

JsonObject& deviceInfo_endPoints_door3 = deviceInfo_endPoints.createNestedObject("Node3");
deviceInfo_endPoints_door3["title"] = "Node 3";
deviceInfo_endPoints_door3["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_door3["units"] = "F";
JsonObject& deviceInfo_endPoints_door3_values = deviceInfo_endPoints_door3.createNestedObject("values");
deviceInfo_endPoints_door3_values["value"] = 0;

JsonObject& deviceInfo_endPoints_door4 = deviceInfo_endPoints.createNestedObject("Node4");
deviceInfo_endPoints_door4["title"] = "Node 4";
deviceInfo_endPoints_door4["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_door4["units"] = "F";
JsonObject& deviceInfo_endPoints_door4_values = deviceInfo_endPoints_door4.createNestedObject("values");
deviceInfo_endPoints_door4_values["value"] = 0;
deviceInfo["description"] = "Temperatures from LoRa Nodes";
deviceInfo["status"] = "ok";

root.printTo(Serial);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
