void txMsg(){
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
  
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["TO"] = call; //TOCALL;
  root["FROM"] = CALLSIGN;
  
  
  root["MESSAGE"] = msg; //COMMENTS; //TEMP;
  root["R"] = 3; //retransmit count starts at 3 and decreases.
  root["PATH"] = "MYCALL,RELAY,WIDE2-2, NOCALL"; //origin of message, each additional hop added to list 
  
  LoRa.beginPacket();
   
  root.printTo(radiopacket);
  LoRa.print(radiopacket);
  Serial.println(radiopacket);
  LoRa.endPacket();
  
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
    
  digitalWrite(LED, LOW);
}

void cq() {

    call = "CQ";
    msg = CQMSG;
    txMsg();
}
