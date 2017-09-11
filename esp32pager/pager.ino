
//If a packet is available, digipeat it.  Otherwise, wait.
void pager(){
  digitalWrite(LED, LOW);
  //Try to receive a reply.
  
    String buf1;
    uint8_t len;
    rssi=0;
  char temp;
    /*
     * When we receive a packet, we repeat it after a random
     * delay if:
     * 1. It asks to be repeated.
     * 2. We've not yet received a different packet.
     * 3. We've waited a random amount of time.
     * 4. The first word is not RT.
     */
     int packetSize = LoRa.parsePacket();
     if (packetSize) {
      digitalWrite(LED, HIGH); //Packet Received

      
     
  while (LoRa.available()) {
    temp = (char)LoRa.read();
    buf1 += temp;
  }
  char buf[buf1.length()+1];
  buf1.toCharArray(buf, buf1.length()+1);
        rssi=LoRa.packetRssi();
        len = buf1.length()+1;
        decodePacket(buf);
        shouldirt();
           }   
}

