
const int latchPin = 8; 
const int clockPin = 12;
const int dataPin = 11;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 1<<row);  // One row LOW
      shiftOut(dataPin, clockPin, MSBFIRST, ~(1<<column));  // One column HIGH
      digitalWrite(latchPin, HIGH);
      delay(60);  //controls speed, fastest visible is about a 25ms delay
    }
  }
}
