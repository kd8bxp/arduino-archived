int x = 0;
int y = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A0, INPUT_PULLUP);
//pinMode(26, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("X: ");
Serial.print(analogRead(0));
x = map(analogRead(0),0,1023,0,100);
Serial.print(" cX: ");
Serial.println(x);
//Serial.print(" Y: ");
//Serial.print(analogRead(26));
//y = map(analogRead(26), 0,4095, -100,100);
//Serial.print(" cY: ");
//Serial.println(y);
delay(500);

}
