void setup() {

  const size_t bufferSize = 9*JSON_OBJECT_SIZE(1) + 9*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(8);
DynamicJsonBuffer jsonBuffer(bufferSize);

JsonObject& root = jsonBuffer.createObject();

JsonObject& deviceInfo = root.createNestedObject("deviceInfo");
deviceInfo["name"] = "Temperatures";

JsonObject& deviceInfo_endPoints = deviceInfo.createNestedObject("endPoints");

JsonObject& deviceInfo_endPoints_1t = deviceInfo_endPoints.createNestedObject("1t");
deviceInfo_endPoints_1t["title"] = "Node 1 Temperature";
deviceInfo_endPoints_1t["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_1t["units"] = "F";
JsonObject& deviceInfo_endPoints_1t_values = deviceInfo_endPoints_1t.createNestedObject("values");
deviceInfo_endPoints_1t_values["value"] = 0;

JsonObject& deviceInfo_endPoints_2t = deviceInfo_endPoints.createNestedObject("2t");
deviceInfo_endPoints_2t["title"] = "Node 2 Temperature";
deviceInfo_endPoints_2t["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_2t["units"] = "F";
JsonObject& deviceInfo_endPoints_2t_values = deviceInfo_endPoints_2t.createNestedObject("values");
deviceInfo_endPoints_2t_values["value"] = 0;

JsonObject& deviceInfo_endPoints_3t = deviceInfo_endPoints.createNestedObject("3t");
deviceInfo_endPoints_3t["title"] = "Node 3 Temperature";
deviceInfo_endPoints_3t["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_3t["units"] = "F";
JsonObject& deviceInfo_endPoints_3t_values = deviceInfo_endPoints_3t.createNestedObject("values");
deviceInfo_endPoints_3t_values["value"] = 0;

JsonObject& deviceInfo_endPoints_4t = deviceInfo_endPoints.createNestedObject("4t");
deviceInfo_endPoints_4t["title"] = "Node 4 Temperature";
deviceInfo_endPoints_4t["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_4t["units"] = "F";
JsonObject& deviceInfo_endPoints_4t_values = deviceInfo_endPoints_4t.createNestedObject("values");
deviceInfo_endPoints_4t_values["value"] = 0;

JsonObject& deviceInfo_endPoints_1h = deviceInfo_endPoints.createNestedObject("1h");
deviceInfo_endPoints_1h["title"] = "Node 1 Humidity";
deviceInfo_endPoints_1h["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_1h["units"] = "";
JsonObject& deviceInfo_endPoints_1h_values = deviceInfo_endPoints_1h.createNestedObject("values");
deviceInfo_endPoints_1h_values["value"] = 0;

JsonObject& deviceInfo_endPoints_2h = deviceInfo_endPoints.createNestedObject("2h");
deviceInfo_endPoints_2h["title"] = "Node 2 Humidity";
deviceInfo_endPoints_2h["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_2h["units"] = "";
JsonObject& deviceInfo_endPoints_2h_values = deviceInfo_endPoints_2h.createNestedObject("values");
deviceInfo_endPoints_2h_values["value"] = 0;

JsonObject& deviceInfo_endPoints_3h = deviceInfo_endPoints.createNestedObject("3h");
deviceInfo_endPoints_3h["title"] = "Node 3 Humidity";
deviceInfo_endPoints_3h["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_3h["units"] = "";
JsonObject& deviceInfo_endPoints_3h_values = deviceInfo_endPoints_3h.createNestedObject("values");
deviceInfo_endPoints_3h_values["value"] = 0;

JsonObject& deviceInfo_endPoints_4h = deviceInfo_endPoints.createNestedObject("4h");
deviceInfo_endPoints_4h["title"] = "Node 4 Humidity";
deviceInfo_endPoints_4h["card-type"] = "crouton-simple-text";
deviceInfo_endPoints_4h["units"] = "";
JsonObject& deviceInfo_endPoints_4h_values = deviceInfo_endPoints_4h.createNestedObject("values");
deviceInfo_endPoints_4h_values["value"] = 0;
deviceInfo["description"] = "Temperatures from LoRa Nodes";
deviceInfo["status"] = "ok";

root.printTo(Serial);
}

void loop() {
  // put your main code here, to run repeatedly:

}
