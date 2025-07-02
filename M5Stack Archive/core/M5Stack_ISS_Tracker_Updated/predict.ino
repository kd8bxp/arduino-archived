//Get the next 5 ISS pass predictions based on your Latitude and Longitude

void decodePassJson(void * parameter ) {
  delay(2000); //short delay before we update people.
  for(;;) {
  getJson(pas);  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

 
  
  count = root["request"]["passes"];
  
  if (count > 5) {count = 5;}
  for (int i=0;i<count; i++){
    
    unsigned int tempEpoch = root["response"][i]["risetime"];
    risetimeUTC[i] = convertEpoch(tempEpoch);
    unsigned int localTime = tempEpoch + gmtOffset_sec;
    risetimeLocal[i] = convertEpoch(localTime);
    duration[i] = root["response"][i]["duration"];
    duration[i] = duration[i] / 60;
      }

   unsigned int timestamp = getCurrentTime();
   lastUpdate = convertEpoch(timestamp) + " " + TZone;
   
       Serial.println("Pass Prediction");
  for (int i=0;i<count; i++) {
    Serial.print(risetimeUTC[i] + " UTC "); Serial.print(risetimeLocal[i] + " " + TZone); Serial.print(" [");Serial.print(duration[i]);Serial.println(" mins.]");
  }
  delay(60000 * 10); //Should be update every 10 minutes
  }
}

//Add a last update time

void displayPass() {
    M5.Lcd.clearDisplay();
    M5.Lcd.setBrightness(200);
    M5.Lcd.setTextColor(0xFFFF);
    M5.Lcd.fillScreen(0x0000);
    M5.Lcd.setFreeFont(FF17);
    M5.Lcd.setCursor(0, 15);
    M5.Lcd.println("Last updated: "+ lastUpdate);
    //M5.Lcd.println("Next fly over..");
    for (int i=0;i<count; i++) {
      M5.Lcd.println(risetimeUTC[i] + " UTC / " + risetimeLocal[i] + " " + TZone +"\nfor [" + duration[i] + " minutes. ]");
    } 
}

