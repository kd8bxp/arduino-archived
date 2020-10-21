void txMsg(){
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
  
  
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["T"] = call; //TO CALL;
  //root["F"] = CALLSIGN; //FROM Call (my call)
  
  
  root["M"] = msg; //Message
  root["R"] = 3; //retransmit count starts at 3 and decreases.
  JsonArray& path = root.createNestedArray("P"); //path of message
  path.add(CALLSIGN); //origin call
  path.add("NOCALL1"); //additional call of each station retransmiting
  path.add("NOCALL2");
  path.add("NOCALL3");
  //origin of message, each additional hop added to list 

if (gps.location.isValid()) {
  JsonArray& gpsCoordinate = root.createNestedArray("C"); //GPS coordinate
  gpsCoordinate.add(gps.location.lat());
  gpsCoordinate.add(gps.location.lng());
}

  LoRa.beginPacket();
   
  root.printTo(radiopacket);
  LoRa.print(radiopacket);
  serialPrt("Transmitting....");
  serialPrt(radiopacket);
  LoRa.endPacket();
  radiopacket="";
  call = "";
  msg = "";
  digitalWrite(LED, LOW);
}

void digipeat(String radiopacket) {
  delay(random(50,400));
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
    
  LoRa.beginPacket();
    
  LoRa.print(radiopacket);
  serialPrt("Digipeating....");
  serialPrt(radiopacket);
  LoRa.endPacket();
    radiopacket="";
    call="";
    msg="";
  digitalWrite(LED, LOW);
}

void beacon() {

    call = "BEACON";
    msg = "DE: " + CALLSIGN + " " + CQMSG;
    txMsg();
}
