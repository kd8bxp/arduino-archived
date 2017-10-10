void updSender(int spd, int cmd, int mode) {

// udpCurrentTimer = millis();

//if (udpCurrentTimer - udpTimer >= 1500) {

 StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
root["spd"] = spd;
root["cmd"] = cmd;
root["mode"] = mode;

char jsonSend[20];
root.printTo(jsonSend);

//snprintf (jsonSend, 20, "%d %d %d", spd, cmd, mode);
//snprintf (jsonSend, 20, "%d", cmd);
char IPtemp[20];
IPremote.toCharArray(IPtemp, 20);
Udp.beginPacket(IPtemp, 8888); //this needs to be updated to use the IP from the MQTT broker topic robothome
    //Udp.write(ReplyBuffer);
    //Udp.print(count, DEC);
    Udp.print(jsonSend);
    Udp.endPacket();
  //delay(500);
 // }
}
