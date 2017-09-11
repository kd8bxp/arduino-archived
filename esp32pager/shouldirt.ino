//Handles retransmission of the packet.
void shouldirt(){
  Serial.println(TO.indexOf(MYCALL));
  //Don't RT any packet containing our own callsign.
  if (TO == CALLSIGN) {
    ledcWriteTone(CHANNEL, 800);
    delay(100);
    ledcWriteTone(CHANNEL, 1200);
    delay(100);
    ledcWriteTone(CHANNEL, 0);
    displaypacket();
  } else if(TO == "BEACON"){
    //Generic Tone, doesn't check to see who/what the packet is for
      ledcWriteTone(CHANNEL, 345);
      delay(100);
      ledcWriteTone(CHANNEL, 600);
      delay(100);
      ledcWriteTone(CHANNEL, 0);
    displaypacket();
  } else if (TO == "CQ") { //calling CQ
     
  } else if (TO == "NET") {
    //calling/starting/talking to a NET
  } else if (TO.indexOf("BLT") >= 0) {
    //display a bulletin
  } else if (TO == "SPCL") {
    //special event stations
  } else if (TO == "SENS") {
    //display sensor information
  } else if (TO == "WX") {
    //display local WX station
  }  else if (TO.indexOf(MYCALL) >= 0) { 
    ledcWriteTone(CHANNEL, 600);
    delay(100);
    ledcWriteTone(CHANNEL, 1200);
    delay(100);
    displaypacket();
  } else {
    digipeat();
  }
   
}
