String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
String topic;
String cmd;
int count = 0;


void  setup() {
Serial.begin(115200);
pinMode(LED_BUILTIN, OUTPUT);
delay(10 * 1000);
Serial.println("{\"cmd\":\"MQTT-SUB\",\"topic\":\"src/myTopic/blink\"}");
Serial.println("{\"cmd\":\"MQTT-SUB\",\"topic\":\"src/myTopic/count\"}");
inputString.reserve(300);
Serial.println("{\"cmd\":\"MQTT-PUB\",\"topic\":\"src/myTopic/time\",\"message\":\"ONLINE\"}");
}
void  loop() {

if (stringComplete) {
    //Serial.println(inputString);
    // clear the string:
    String msg2 = getValue(inputString, ':', 2);
    topic = getValue(msg2, ',', 0);
    topic = removeQuotes(topic);
    topic = getValue(topic, '/', 2);
    cmd = getValue(inputString, ':', 3);
    cmd = removeQuotes(cmd);
    cmd = removeBrace(cmd);
    //Serial.println(topic);
    //Serial.println(cmd);
    inputString = "";
    stringComplete = false;
    if (topic == "blink") {   blink();     }
    if (topic == "count") {
      Serial.println("{\"cmd\":\"MQTT-PUB\",\"topic\":\"src/myTopic/time\",\"message\":"+String(count)+"}");
      count++;
    }
  }
}


void blink() {
  if (cmd == "1") {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("{\"cmd\":\"MQTT-PUB\",\"topic\":\"src/myTopic/time\",\"message\":\"ON\"}");
  }
  if (cmd == "0") {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("{\"cmd\":\"MQTT-PUB\",\"topic\":\"src/myTopic/time\",\"message\":\"OFF\"}");
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
    //if (inChar == '\n') {
    if (inChar == '}') {
      stringComplete = true;
    }
  }
}

//found at https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


//based on code found at: https://stackoverflow.com/questions/40093116/remove-character-from-string-c-arduino-uno
String removeQuotes(String data) {
  char c;
    char no = '\"'; //character I want removed.

    for (int i=0; i<data.length();++i){
        c = data.charAt(i);
        if(c==no){
            data.remove(i, 1);
        }
    }
    return data;
}

String removeBrace(String data) {
  char c;
    char no = '}'; //character I want removed.

    for (int i=0; i<data.length();++i){
        c = data.charAt(i);
        if(c==no){
            data.remove(i, 1);
        }
    }
    return data;
}
