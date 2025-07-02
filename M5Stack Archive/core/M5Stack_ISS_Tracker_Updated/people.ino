//Get People in Space, how many, their names and the craft they are on board 

void decodePeopleJson(void * parameter) {
delay(5000); //we will wait about 5 seconds the first time we ask.
  for(;;) {
  getJson(ppl);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  number = root["number"];
  if (number > 10) {number = 10;}
  for (int i=0;i<number; i++){
    
    const char* temp1 = root["people"][i]["name"];
    const char* temp2 = root["people"][i]["craft"];
    name[i] = (String)temp1;
    craft[i] = (String)temp2;
  }
   Serial.print("There are "); Serial.print(number); Serial.println(" people in space right now.");
  for (int i=0;i<number; i++) {
    Serial.print(name[i]);Serial.print(" on board ");Serial.println(craft[i]);
  }
  delay((unsigned long)(60 * 60 *1000)); //should be update once an hour
 }
}

void displayPPL() {
    M5.Lcd.clearDisplay();
    M5.Lcd.setBrightness(200);
    M5.Lcd.setTextColor(0xFFFF);
    M5.Lcd.fillScreen(0x0000);
    M5.Lcd.setFreeFont(FF17);
    M5.Lcd.setCursor(15, 15);
    M5.Lcd.print("There are " + (String)number);
    M5.Lcd.println(" People in Space!");
    
    for (int i=0;i<number; i++) {
      M5.Lcd.println(name[i] + " on board " + craft[i]);
    } 
} 
