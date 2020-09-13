void txMsg(){
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
  
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["TO"] = call; //TOCALL;
  root["FROM"] = CALLSIGN;
  
  
  root["MESSAGE"] = msg; //COMMENTS; //TEMP;
  root["R"] = 3; //retransmit count starts at 3 and decreases.
  JsonArray& path = root.createNestedArray("PATH");
  path.add(CALLSIGN);
  path.add("NOCALL1");
  path.add("NOCALL2");
  path.add("NOCALL3");
  //origin of message, each additional hop added to list 
  
  LoRa.beginPacket();
   
  root.printTo(radiopacket);
  LoRa.print(radiopacket);
  Serial.println(radiopacket);
  LoRa.endPacket();
  radiopacket="";
  call = "";
  msg = "";
  //packetnum++;
  digitalWrite(LED, LOW);
}

void digipeat() {

  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
    
  LoRa.beginPacket();
    
  LoRa.print(radiopacket);
  Serial.println(radiopacket);
  LoRa.endPacket();
    radiopacket="";
    call="";
    msg="";
  digitalWrite(LED, LOW);
}

void beacon() {

    call = "BEACON";
    msg = CQMSG;
    txMsg();
}
