void txMsg(){
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
  
  
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["T"] = call; //TOCALL;
  //root["F"] = CALLSIGN; //FROM Call (my call)
  
  
  root["M"] = msg; //COMMENTS; //TEMP;
  root["R"] = 3; //retransmit count starts at 3 and decreases.
  JsonArray& path = root.createNestedArray("P");
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
  digitalWrite(LED, LOW);
}

void digipeat(String radiopacket) {
  delay(random(50,400));
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
    msg = "DE: " + CALLSIGN + " " + CQMSG;
    txMsg();
}
