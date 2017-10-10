void checkUDP() {
  yield();
  // if there's data available, read a packet

   udpCurrentTimer = millis();

if (udpCurrentTimer - udpTimer >= 500) {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
   // Serial.print("Received packet of size ");
   // Serial.println(packetSize);
    //Serial.print("From ");
   // IPAddress remote = Udp.remoteIP();
    //for (int i = 0; i < 4; i++)
    //{
      //Serial.print(remote[i], DEC);
      //if (i < 3)
      //{
      //  Serial.print(".");
      //}
    //}
    //Serial.print(", port ");
    //Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    //Serial.println("Contents:");
    //Serial.println(packetBuffer);
    decodePacket(packetBuffer);
    // send a reply, to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
  }
} 
  //delay(100);
  Udp.flush();
}

/* 
 *  This is where the robot will receive the commands from the remote
 */
