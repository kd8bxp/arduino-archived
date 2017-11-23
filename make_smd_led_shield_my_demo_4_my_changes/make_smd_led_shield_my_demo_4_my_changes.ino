const int latchPin = 8; 
const int clockPin = 12;
const int dataPin = 11;

void setup() {
  // put your setup code here, to run once:
 pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 1<<row);  // One row LOW
      shiftOut(dataPin, clockPin, MSBFIRST, (1<<column));  // One column HIGH
      digitalWrite(latchPin, HIGH);
      delay(50);  //controls speed, fastest visible is about a 25ms delay
    }
  }
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 1<<row);  // One row LOW
      shiftOut(dataPin, clockPin, LSBFIRST, (1<<column));  // One column HIGH
      digitalWrite(latchPin, HIGH);
      delay(50);  //controls speed, fastest visible is about a 25ms delay
    }
  }
}
