
void title() {
  M5.Lcd.setBrightness(200);
  M5.Lcd.setCursor(63,15);
  M5.Lcd.setFreeFont(FF17);
  M5.Lcd.println("Welcome to ISS Tracker");
  M5.Lcd.setCursor(60,35);
  M5.Lcd.println("by LeRoy Miller, (c)2018");
  M5.Lcd.setCursor(25,55);
  M5.Lcd.println("Based on code by Giuliano Pisoni");
  M5.Lcd.println("\nAPIs used:");
  M5.Lcd.println("Google Static Maps");
  M5.Lcd.println("Open-notify.org");
  M5.Lcd.println("timezonedb.com");
  M5.Lcd.println("ip-api.com\n");
  //M5.Lcd.println("wheretheiss.at");
  delay(5000);
}

//http://c.tile.stamen.com/toner/#10/39.5152/-84.3971 posiable map alterntive uses png. http://c.tile.stamen.com/watercolor/#10/39.5152/-84.3971 uses jpgs
