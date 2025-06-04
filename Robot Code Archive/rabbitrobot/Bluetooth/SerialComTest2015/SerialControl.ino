int led = 13;
boolean ledStatus = false;
 
void setup() {                
  Serial.begin(9600);
  pinMode(led, OUTPUT);     
}
 
void loop() {
  if (Serial.available()) {
    int inChar = getChar();
    if (inChar == 'D') {
      Serial.println("This is some test data|");
    } else if(inChar == 'L') {
      ledStatus = !ledStatus;
      if (ledStatus) {
        Serial.prntln("Turning light On.|");
        digitalWrite(led, HIGH); 
      } else {
        Serial.prntln("Turning light On.|");
        digitalWrite(led, LOW);
      }
    } else {
       Serial.println("Unknown command received.|"); 
    }
  }
}
 
char getChar() {
  while(Serial.available() == 0);
  return((char)Serial.read());
}

