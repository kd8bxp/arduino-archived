String msg = "{cmd\":\"MQTT-PUB\",\"topic\":\"src/myTopic/blink\",\"message\":\"1\"}";

void setup() {
  Serial.begin(115200);
  String msg2 = getValue(msg, ':', 2);
  String topic = getValue(msg2, ',', 0);
  Serial.println(topic);
  topic = removeQuotes(topic);
  Serial.println(topic);
  topic = getValue(topic, '/', 2);
  Serial.println(topic);
  //Serial.println(topic);
  String cmd = getValue(msg, ':', 3);
  cmd = removeQuotes(cmd);
  cmd = removeBrace(cmd);
  Serial.println(cmd);
  //Serial.println(getValue(msg, ':', 2));
}

void loop() {
  
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
