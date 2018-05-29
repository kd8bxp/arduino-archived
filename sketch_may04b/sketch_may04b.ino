int e1 = 11; //10;
int m1 = 13; //12;

void setup() {
pinMode(m1, OUTPUT);


}

void loop() {
digitalWrite(m1, LOW);
analogWrite(e1, 100);
delay(10000);
analogWrite(e1,0);
while(1);
}
