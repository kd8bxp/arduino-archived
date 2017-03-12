uint8_t curkey = 0;

extern "C" {
  uint16_t getpc();
  uint8_t getop();
  void exec6502(int32_t tickcount);
  void reset6502();
  void serout(uint8_t val) {
    Serial.write(val);
  }
  uint8_t getkey() {
    return(curkey);
  }
  void clearkey() {
    curkey = 0;
  }
  void printhex(uint16_t val) {
    Serial.print(val, HEX);
    Serial.println();
  }
}

void setup () {
  Serial.begin (9600);
  Serial.println ();

  reset6502();
}

void loop () {
  exec6502(100); //if timing is enabled, this value is in 6502 clock ticks. otherwise, simply instruction count.
  if (Serial.available()) {
    curkey = Serial.read() & 0x7F;
  }
}

