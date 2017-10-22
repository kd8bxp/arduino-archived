void checkUDP() {
 yield();
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
   
    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    decodePacket(packetBuffer);
  } else {
      stop();
      cmd = 0;
        }
  Udp.flush();
}

/* 
 *  This is where the robot will receive the commands from the remote
 */
