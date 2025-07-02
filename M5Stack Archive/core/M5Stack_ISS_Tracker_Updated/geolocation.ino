/*
 * This function will use the external IP to get a Latitude and Longitude 
 * which will be used for many different things for ISS tracker.
 * The website http://ip-api.com is used for this.
 */

void geolocation(){

    String url;
    url = "http://ip-api.com/json";
    getJson(url);
 
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);
  JsonObject& root = jsonBuffer.parseObject(payload);
  root.printTo(Serial);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // Extract values
  Serial.println(("Response:"));
  Serial.println(root["lat"].as<char*>());
  Serial.println(root["lon"].as<char*>());
  coordinate = String(root["lat"].as<char*>()) + "," + String(root["lon"].as<char*>());
  coordinatePrev = "lat=" + String(root["lat"].as<char*>()) + "&lon=" + String(root["lon"].as<char*>()); //do we use this anywhere (?) 
  coordinateTZ = "lat=" + String(root["lat"].as<char*>()) + "&lng=" + String(root["lon"].as<char*>());
  mylat = root["lat"];
  mylon = root["lon"];
  M5.Lcd.println("My Lat/Lon: "+coordinate);
}
