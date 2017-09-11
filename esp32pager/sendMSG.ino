void sendMSG(){
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
  //float vcc=voltage();
  //Serial.println("Transmitting..."); // Send a message to rf95_server
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["TO"] = TO;
  root["FROM"] = MYCALL;
  root["MESSAGE"] = MSG1;
  root["RT"] = "";
  
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
