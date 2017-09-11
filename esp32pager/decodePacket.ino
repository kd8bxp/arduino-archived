void decodePacket(String pkt) {
  StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(pkt);
Serial.println(pkt);
if (!root.success()) {
    Serial.println("parseObject() failed");
    
  }

 // Clear the buffer.
 String to = root["TO"];
 String from = root["FROM"];
 String msg1 = root["MESSAGE"];
 //String rt = root["RT"];
 TO = to;
 FROM = from;
 MSG1 = msg1;
 //RT = rt;
 RTCNT = root["CFG"][0]["RTCOUNT"];
 if (RTCNT > 0) {
  for (int xx=0; xx<RTCNT; xx++) {
    RTCALL[xx] = root["RT"][xx]["CALL"];
    RTRSSI[xx] = root["RT"][xx]["RSSI"];
    RTDEVICEID[xx] = root["RT"][xx]["DEVICEID"];
    RTDEVICETYPE[xx] = root["RT"][xx]["DEVICETYPE"];
    RTTIME[xx] = root["RT"][xx]["RTTIME"];
    RTDATE[xx] = root["RT"][xx]["RTDATE"];
  }
 }
 GRID = root["CFG"][0]["GRID"];
 TYPE = root["CFG"][0]["TYPE"];
 OTHER = root["CFG"][0]["OTHER"];
 TXDEVICEID = root["CFG"][0]["DEVICEID"];
 TXTIME = root["TIME"];
 TXDATE = root["DATE"];
}

