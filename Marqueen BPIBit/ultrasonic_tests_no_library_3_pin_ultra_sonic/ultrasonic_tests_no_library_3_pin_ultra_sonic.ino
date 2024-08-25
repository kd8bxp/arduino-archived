volatile long RangeInInches;
long duration;// the Pulse time received;
#define trigPin P1


void setup() {
  Serial.begin(115200);
  
}

void loop() {
  
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin,LOW);
  pinMode(trigPin,INPUT);
  duration = pulseIn(trigPin,HIGH);
  RangeInInches = duration/74/2;
Serial.println(RangeInInches);
delay(100);


}
