
void setup() {
  // put your setup code here, to run once:

pinMode(A0, INPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
digitalWrite(5, LOW);
digitalWrite(6, LOW);
digitalWrite(7, LOW);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(analogRead(A0));
digitalWrite(5, HIGH);
delay(500);
digitalWrite(5, LOW);
digitalWrite(6, HIGH);
delay(500);
digitalWrite(6,LOW);
digitalWrite(7,HIGH);
delay(500);
digitalWrite(7,LOW);
delay(200);
digitalWrite(5, HIGH);
digitalWrite(6, HIGH);
digitalWrite(7, HIGH);
delay(500);
digitalWrite(5, LOW);
digitalWrite(6, LOW);
digitalWrite(7, LOW);
delay(500);
for (int i=0; i<255; i++) {
  analogWrite(5, i);
 // analogWrite(6, i);
 // analogWrite(7, i);
  delay(25);
}
for (int i=254; i>0; i--) {
  analogWrite(5, i);
  //analogWrite(6, i);
  //analogWrite(7, i);
  delay(25);
}
//delay(500);


}
