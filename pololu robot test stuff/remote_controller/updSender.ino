void updSender(int spd, int cmd, int mode) {

 udpCurrentTimer = millis();

if (udpCurrentTimer - udpTimer >= 500) {

 StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
root["spd"] = spd;
root["cmd"] = cmd;
root["mode"] = mode;

String jsonSend;
root.printTo(jsonSend);
Udp.beginPacket("10.11.0.101", 8888); //this needs to be updated to use the IP from the MQTT broker topic robothome
    //Udp.write(ReplyBuffer);
    //Udp.print(count, DEC);
    Udp.print(jsonSend);
    Udp.endPacket();
  //delay(500);
  }
}
