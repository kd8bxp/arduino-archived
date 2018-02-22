void setup() {
  const size_t bufferSize = 9*JSON_OBJECT_SIZE(1) + 9*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(8) + 810;
DynamicJsonBuffer jsonBuffer(bufferSize);

const char* json = "{\"deviceInfo\":{\"name\":\"Temperatures\",\"endPoints\":{\"1t\":{\"title\":\"Node 1 Temperature\",\"card-type\":\"crouton-simple-text\",\"units\":\"F\",\"values\":{\"value\":0}},\"2t\":{\"title\":\"Node 2 Temperature\",\"card-type\":\"crouton-simple-text\",\"units\":\"F\",\"values\":{\"value\":0}},\"3t\":{\"title\":\"Node 3 Temperature\",\"card-type\":\"crouton-simple-text\",\"units\":\"F\",\"values\":{\"value\":0}},\"4t\":{\"title\":\"Node 4 Temperature\",\"card-type\":\"crouton-simple-text\",\"units\":\"F\",\"values\":{\"value\":0}},\"1h\":{\"title\":\"Node 1 Humidity\",\"card-type\":\"crouton-simple-text\",\"units\":\"\",\"values\":{\"value\":0}},\"2h\":{\"title\":\"Node 2 Humidity\",\"card-type\":\"crouton-simple-text\",\"units\":\"\",\"values\":{\"value\":0}},\"3h\":{\"title\":\"Node 3 Humidity\",\"card-type\":\"crouton-simple-text\",\"units\":\"\",\"values\":{\"value\":0}},\"4h\":{\"title\":\"Node 4 Humidity\",\"card-type\":\"crouton-simple-text\",\"units\":\"\",\"values\":{\"value\":0}}},\"description\":\"Temperatures from LoRa Nodes\",\"status\":\"ok\"}}";

JsonObject& root = jsonBuffer.parseObject(json);

JsonObject& deviceInfo = root["deviceInfo"];
const char* deviceInfo_name = deviceInfo["name"]; // "Temperatures"

JsonObject& deviceInfo_endPoints = deviceInfo["endPoints"];

JsonObject& deviceInfo_endPoints_1t = deviceInfo_endPoints["1t"];
const char* deviceInfo_endPoints_1t_title = deviceInfo_endPoints_1t["title"]; // "Node 1 Temperature"
const char* deviceInfo_endPoints_1t_card_type = deviceInfo_endPoints_1t["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_1t_units = deviceInfo_endPoints_1t["units"]; // "F"

int deviceInfo_endPoints_1t_values_value = deviceInfo_endPoints_1t["values"]["value"]; // 0

JsonObject& deviceInfo_endPoints_2t = deviceInfo_endPoints["2t"];
const char* deviceInfo_endPoints_2t_title = deviceInfo_endPoints_2t["title"]; // "Node 2 Temperature"
const char* deviceInfo_endPoints_2t_card_type = deviceInfo_endPoints_2t["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_2t_units = deviceInfo_endPoints_2t["units"]; // "F"

int deviceInfo_endPoints_2t_values_value = deviceInfo_endPoints_2t["values"]["value"]; // 0

JsonObject& deviceInfo_endPoints_3t = deviceInfo_endPoints["3t"];
const char* deviceInfo_endPoints_3t_title = deviceInfo_endPoints_3t["title"]; // "Node 3 Temperature"
const char* deviceInfo_endPoints_3t_card_type = deviceInfo_endPoints_3t["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_3t_units = deviceInfo_endPoints_3t["units"]; // "F"

int deviceInfo_endPoints_3t_values_value = deviceInfo_endPoints_3t["values"]["value"]; // 0

JsonObject& deviceInfo_endPoints_4t = deviceInfo_endPoints["4t"];
const char* deviceInfo_endPoints_4t_title = deviceInfo_endPoints_4t["title"]; // "Node 4 Temperature"
const char* deviceInfo_endPoints_4t_card_type = deviceInfo_endPoints_4t["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_4t_units = deviceInfo_endPoints_4t["units"]; // "F"

int deviceInfo_endPoints_4t_values_value = deviceInfo_endPoints_4t["values"]["value"]; // 0

JsonObject& deviceInfo_endPoints_1h = deviceInfo_endPoints["1h"];
const char* deviceInfo_endPoints_1h_title = deviceInfo_endPoints_1h["title"]; // "Node 1 Humidity"
const char* deviceInfo_endPoints_1h_card_type = deviceInfo_endPoints_1h["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_1h_units = deviceInfo_endPoints_1h["units"]; // ""

int deviceInfo_endPoints_1h_values_value = deviceInfo_endPoints_1h["values"]["value"]; // 0

JsonObject& deviceInfo_endPoints_2h = deviceInfo_endPoints["2h"];
const char* deviceInfo_endPoints_2h_title = deviceInfo_endPoints_2h["title"]; // "Node 2 Humidity"
const char* deviceInfo_endPoints_2h_card_type = deviceInfo_endPoints_2h["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_2h_units = deviceInfo_endPoints_2h["units"]; // ""

int deviceInfo_endPoints_2h_values_value = deviceInfo_endPoints_2h["values"]["value"]; // 0

JsonObject& deviceInfo_endPoints_3h = deviceInfo_endPoints["3h"];
const char* deviceInfo_endPoints_3h_title = deviceInfo_endPoints_3h["title"]; // "Node 3 Humidity"
const char* deviceInfo_endPoints_3h_card_type = deviceInfo_endPoints_3h["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_3h_units = deviceInfo_endPoints_3h["units"]; // ""

int deviceInfo_endPoints_3h_values_value = deviceInfo_endPoints_3h["values"]["value"]; // 0

JsonObject& deviceInfo_endPoints_4h = deviceInfo_endPoints["4h"];
const char* deviceInfo_endPoints_4h_title = deviceInfo_endPoints_4h["title"]; // "Node 4 Humidity"
const char* deviceInfo_endPoints_4h_card_type = deviceInfo_endPoints_4h["card-type"]; // "crouton-simple-text"
const char* deviceInfo_endPoints_4h_units = deviceInfo_endPoints_4h["units"]; // ""

int deviceInfo_endPoints_4h_values_value = deviceInfo_endPoints_4h["values"]["value"]; // 0

const char* deviceInfo_description = deviceInfo["description"]; // "Temperatures from LoRa Nodes"
const char* deviceInfo_status = deviceInfo["status"]; // "ok"
}

void loop() {
  // put your main code here, to run repeatedly:

}
