/*
 * Unfortuntally I couldn't get this to work
 *  ** Now I remember why I didn't use wheretheiss.at for the first project **
 * Note to self, if I figure it out, besure to reenable the task, and change the mode count
 * 
void decodeWhereAT( void * parameter) {
  delay(1500); //short delay before starting.
  
  for(;;) {
    if (mode == 3) {
    getJson("https://api.wheretheiss.at/v1/satellites/25544");
   // const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  //DynamicJsonBuffer jsonBuffer(capacity);
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  float templat, templon;
  int tempalt, tempspdKPH, tempfp;
  unsigned int tempTS;
  String tempVisable;
  
  templat = root["latitude"];
  templon = root["longitude"];
  tempalt = root["altitude"];
  tempspdKPH = root["velocity"];
  tempTS = root["timestamp"];
  tempfp = root["footprint"];
  const char* temp1 = root["visibility"];
  tempVisable = (String)temp1;
  getDistance();
  String currentTime = convertEpoch(tempTS) + " " + TZone;
  M5.Lcd.setFreeFont(FF17);
  M5.Lcd.setCursor(10,15);
  M5.Lcd.println(currentTime);
  M5.Lcd.println("Located: " + (String)templat + "," + (String)templon);
  M5.Lcd.println("Distance: " + (String)distance + " miles");
  M5.Lcd.println("Altitude: " + (String)tempalt);
  M5.Lcd.println("Speed (KPH): " + (String)tempspdKPH);
  M5.Lcd.println("Speed (MPH): " + (String)(tempspdKPH * 0.6213711922));
  M5.Lcd.println("Footprint: " + (String)tempfp);
  M5.Lcd.println("ISS is in: " + tempVisable);
  
  delay(5000); //Update display every 5 seconds - rate limit is once per second, so should be good here.
  }
 }
}
*/
