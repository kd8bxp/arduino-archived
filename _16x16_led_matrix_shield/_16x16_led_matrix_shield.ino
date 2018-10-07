#define CLOCK 8
#define DATA 7
#define LATCH 9
#define ROWA 5
#define ROWB 4
#define ROWC 3
#define ROWD 2

unsigned int pattern[16] = {
 0b0010000001111100,
 0b0110000010000010,
 0b0010000000000010,
 0b0010000001111100,
 0b0010000010000000,
 0b0010000010000000,
 0b0111000011111110,
 0b0000000000000000,
 0b0111110000001100,
 0b1000001000010100,
 0b0000001000100100,
 0b0011110001000100,
 0b0000001010000100,
 0b1000001011111110,
 0b0111110000000100,
 0b0000000000000000};
 
void setup() {
 pinMode (CLOCK, OUTPUT);
 pinMode (DATA, OUTPUT);
 pinMode (LATCH, OUTPUT);
 pinMode (ROWA, OUTPUT);
 pinMode (ROWB, OUTPUT);
 pinMode (ROWC, OUTPUT);
 pinMode (ROWD, OUTPUT);
}

void loop() {
 for (byte i=0; i<16; i++) {
   digitalWrite(LATCH, LOW);
   shiftOut(DATA, CLOCK, MSBFIRST, 0);
   shiftOut(DATA, CLOCK, MSBFIRST, 0);
   digitalWrite(LATCH, HIGH);
   digitalWrite(ROWA, bitRead(i, 0));
   digitalWrite(ROWB, bitRead(i, 1));
   digitalWrite(ROWC, bitRead(i, 2));
   digitalWrite(ROWD, bitRead(i, 3));
   digitalWrite(LATCH, LOW);
   shiftOut(DATA, CLOCK, MSBFIRST, highByte(pattern[i]));
   shiftOut(DATA, CLOCK, MSBFIRST, lowByte(pattern[i]));
   digitalWrite(LATCH, HIGH);
   delay(8);
 }
}

