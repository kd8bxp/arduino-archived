/*
 * Used to get the GMT offset for your timezone
 */

 void timeZone() {
 String url;
 url = "http://api.timezonedb.com/v2/get-time-zone?key=" + TZAPIKEY + "&format=json&fields=gmtOffset,abbreviation&by=position&"+coordinateTZ;
 getJson(url);
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  gmtOffset_sec = root["gmtOffset"];
  const char* temp1 = root["abbreviation"];
  TZone = (String)temp1;
  Serial.print("GMT Offset = ");
  Serial.print(gmtOffset_sec);
  Serial.println(" " + TZone);
  M5.Lcd.println("GMT Offset = "+(String)gmtOffset_sec);
}

unsigned int getCurrentTime() {
  String url;
  url = "http://api.timezonedb.com/v2/get-time-zone?key=" + TZAPIKEY + "&format=json&fields=timestamp&by=position&"+coordinateTZ;
 getJson(url);
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return 0;
  }
  unsigned int timestamp;
  timestamp = root["timestamp"];
  return (timestamp);
}

