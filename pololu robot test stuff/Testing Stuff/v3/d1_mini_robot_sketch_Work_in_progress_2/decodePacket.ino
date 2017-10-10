void decodePacket(char* msg1) {

  /* StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(msg1);
//Serial.println(msg1);
if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
      }

setSpeed = root["spd"];
cmd = root["cmd"];
mode = root["mode"];
*/

setSpeed = 100;
mode = 0;
cmd = (int)msg1;

/*Serial.print("setSpeed: ");
Serial.print(setSpeed);
Serial.print(" Cmd: ");
Serial.print(cmd);
Serial.print(" Mode: ");
Serial.println(mode);*/
yield();

}
