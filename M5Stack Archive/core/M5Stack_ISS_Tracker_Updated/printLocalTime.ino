void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  M5.Lcd.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void displayTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  M5.Lcd.setCursor(0,16);
  M5.Lcd.print(distance);
  M5.Lcd.print(" miles");
  M5.Lcd.setCursor(225,16);
  M5.Lcd.print(&timeinfo, "%H:%M:%S");
}


