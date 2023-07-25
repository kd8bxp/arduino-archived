

#define M5STACKFIRE_MICROPHONE_PIN 34

int micValue = 0;
int capture[144]; 

void setup() {
    Serial.begin(115200);
    //pinMode(M5STACKFIRE_MICROPHONE_PIN, INPUT_PULLUP);
}

int count = 0;
void loop() {
    //count++;
    for (int i=0; i<144; i++) {
     int readMicValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);
     // int temp = map(readMicValue, 1000, 2200, 0, 140);
      micValue = readMicValue; //temp;
      if (micValue > 1950 && micValue < 2250) { readMicValue = 1950; }
      capture[i] = readMicValue;
      //if (micValue <= 1950 ) {micValue = 2050;} else {micValue =+ micValue;}
   }
    
    //micAvg = micValue / 5;
    //if (count > 3) {
    //  count = 0;
    //micValue = 0; //2050;
    //}
    
//if (micValue < 1950) {micValue = 2050;}
for (int i=0; i<144; i++) {
    Serial.println(capture[i] / 50  );
}
 //while(1);
    delay(1);
}
