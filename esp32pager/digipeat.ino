void digipeat(){
  //Check if count is too high and check if I have already RT.
  bool haveirt = haveialready(); //check to see if I have already RT the package
  if (haveirt == false) {
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
  //float vcc=voltage();
  //Serial.println("Transmitting..."); // Send a message to rf95_server
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["TO"] = TO;
  root["FROM"] = FROM;
  root["MESSAGE"] = MSG1;
  root["RT"] = MYCALL + " rssi=" + (String)rssi;
   
  //Serial.println("Sending..."); delay(10);
  LoRa.beginPacket();
  //LoRa.print(radiopacket);
  String radiopacket;
  root.printTo(radiopacket);
  display.clear();
  display.setColor(WHITE);
  display.drawString(20, 15, "Sending...");
  display.display();
  LoRa.print(radiopacket);
  Serial.println(radiopacket);
  LoRa.endPacket();
  delay(500);
  display.clear();
  display.display();
  digitalWrite(LED, LOW);
  }
}

bool haveialready() {
  if (RTCNT < 6) { 
   for (int xx=0; xx < RTCNT;xx++) {
    if (RTCALL[xx].indexOf(MYCALL) >= 0) {return true;} else { return true; }
   }
}
return false;
}

