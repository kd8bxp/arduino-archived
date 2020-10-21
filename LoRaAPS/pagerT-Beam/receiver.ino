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
  rssi = String(LoRa.packetRssi(),DEC);
  char buf[buf1.length()+1];
  buf1.toCharArray(buf, buf1.length()+1);
  checkPacket(buf);     
           }  
}

void checkPacket(String pkt) {

if (pkt.indexOf(CALLSIGN) > 0) {
  
     StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(pkt);

  // Test if parsing succeeds.
  if (!root.success()) {
    serialPrt("parseObject() failed");
    return;
  }
  //Our Callsign was found, now let's see if we sent the message. Could probably also check the "P" or path of the json string
  if (root["P"][0] != CALLSIGN) {
  from = root["P"][0].as<String>();
  holdMsg = root["M"].as<String>();
  for (int i=1;i<4;i++){
  path[i] = root["P"][i].as<String>();
  }
  displayChange = 2;
  displayMsg(from,holdMsg); //Display message
  }
  
} else {
   //This is where we decode the message to get retransmit count, and to add our call if needed, and then to retransmit the message
   StaticJsonBuffer<500> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(pkt);

   //Check for BLT, BEACON, CQ, WX special callsigns
   String t = root["T"];
   if (t == "BEACON" || t == "CQ" || t == "WX" || t == "BLN") {
    from = root["P"][0].as<String>();
    String temp = from + "\n" + t;
    holdMsg = root["M"].as<String>();
    displayChange = 2;
    displayMsg(temp,holdMsg);
   }
   int rtcount = root["R"];
   int count = rtcount;
   
   if (count > 0) {
   root["P"][4-count]=CALLSIGN;
   count--; // = rtcount - 1; 
   root["R"] = count;
   
   String radiopacket;
  root.printTo(radiopacket);
  digipeat(radiopacket);
  }
 }
}
