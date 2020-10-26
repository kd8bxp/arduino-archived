
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete




void  setup() {
Serial.begin(115200);
delay(5000);
Serial.println("{\"cmd\":\"MQTT-SUB\",\"topic\":\"src/myTopic/blink\"}");
inputString.reserve(200);
}
void  loop() {
////Serial.println("{[1,\"src/myTopic/time\","+String(millis())+"]}");
//Serial.println("{\"cmd\":\"MQTT-PUB\",\"topic\":\"src/myTopic/time\",\"message\":"+String(millis())+"}");
////Serial.println("{\"cmd\":\"MQTT-PUB\",\"topic\":\"src/myTopic/time\",\"message\":"+String(millis())+"}");
////Serial.println("{[cmd:MQTT-PUB],[topic:src/myTopic/time],[message:"+String(millis())+"]}");
//delay(1000);
if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
