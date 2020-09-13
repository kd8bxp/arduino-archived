void rx() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
        digitalWrite(LED, LOW);
        String buf1;
        char temp;
        digitalWrite(LED, HIGH); //Packet Received
    // read packet
     while (LoRa.available()) {
    temp = (char)LoRa.read();
    buf1 += temp;
  }
  char buf[buf1.length()+1];
  buf1.toCharArray(buf, buf1.length()+1);
  Serial.println(buf);
  checkPacket(buf);     
           }  
}

void checkPacket(String pkt) {

Serial.println(pkt); //debug to see if we really got a packet

if (pkt.indexOf(CALLSIGN) > 0) {
  
     StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(pkt);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
 // For other devices this is were the message can be displayed for a Relay, this is where the message stops if addressed to the relay call sign (no one will probably ever see this message)
 // also check to see if this device retransmitted the message, if it retransmitted, then it should NOT display the message
} else {
   //This is where we decode the message to get retransmit count, and to add our call if needed, and then to retransmit the message
   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(pkt);
   int rtcount = root["R"];
   if (rtcount > 0) {
   root["PATH"][4-rtcount]=CALLSIGN;
   rtcount = rtcount - 1; 
   root["R"] = rtcount;
   
   String radiopacket;
  root.printTo(radiopacket);
  //Serial.println(radiopacket);
  digipeat();
  }
 }
}
