void displayIntro() {
  display.clear();
  display.setColor(WHITE);
  display.drawString(0,0,"My Call: ");
  display.drawString(45, 0,MYCALL);
  display.drawString(35,11,"LoRaHam Pager");
  display.drawString(47,22,"by KK4VCZ.");
  display.drawString(32,33,"ESP32 Pager by");
  display.drawString(50,44,"KD8BXP.");
  display.display();
  delay(3000); //while delayed can't get messages
  display.clear();
  display.drawString(0,30,"SSID: ");
  display.drawString(30,30,ssid);
  display.display();
  delay(3000);
  display.clear();
  display.display();
  
}

