void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  if ((String)topic == "robothome") {
    for (int i=0; i < length; i++) {
      subclientid[i] = (char)payload[i];
    }
    client.subscribe(subclientid);
    client.unsubscribe("robothome");
  }
  display.clear();
  String msg;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  char msg2[200];
  msg.toCharArray(msg2, 200);
  decodePacket(msg2);
  //display.setFont(ArialMT_Plain_10);
  //display.drawStringMaxWidth(35,13,60,msg); //d1 mini
  //display.display();
  Serial.println();

}
