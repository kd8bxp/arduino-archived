struct TwoNibbles {
  uint8_t  v1: 4;
  uint8_t  v2: 4;
}  tn;

word low = 1001;
byte test = B00001001;

void setup() {
  Serial.begin(9600);
  tn.v1 = B00000001;
  tn.v2 = 0xA;
  Serial.print(F("tn.v1 = 0b"));
  Serial.print(tn.v1); Serial.print(" ");
  Serial.println(tn.v1, BIN);
  Serial.print(F("tn.v2 = 0b"));
  Serial.println(tn.v2, BIN);
  Serial.println(lowByte(B00001001), BIN);
  Serial.println(highByte(B00001001), BIN);
  Serial.println(low);
  bitWrite(low, 2, HIGH);
  Serial.println(low);
  Serial.println(lowByte(test), BIN);
  bitWrite(test, 2, HIGH);
  Serial.println(lowByte(test), BIN);
  Serial.println(15, BIN);
  Serial.println(lowByte(15), BIN);
}
void loop() {}
