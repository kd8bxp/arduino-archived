
int dataPin = 2;
int latchPin = 4;
int clockPin = 3;

byte dec_digits[] = {0b10000110,0b01101101,0b1001111,0b01010110,0b011011011,0b1110011,0b00001110,0b11111111,0b01011110,0b0111111 };

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  for (int numberToDisplay = 0; numberToDisplay < 10; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[numberToDisplay]); 
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(1000);
  }
}


