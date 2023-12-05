#define pinl 1
#define pinr 2

void setup() {
  // put your setup code here, to run once:
pinMode(pinl, INPUT);
pinMode(pinr, INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("L: ");
Serial.print(analogRead(pinl));
Serial.print(" R: ");
Serial.println(analogRead(pinr));
delay(250);
}
