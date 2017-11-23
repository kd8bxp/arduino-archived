
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int frames = 6;
int i = 0;
int waitCount = 0;
int waitCycles = 85;

byte columns[] = {
  0b01111111, 0b10111111, 0b11011111, 0b11101111,
  0b11110111, 0b11111011, 0b11111101, 0b11111110
};

byte sequence[][8] = {
{255,129,129,129,129,129,129,255} , 
{0,126,66,66,66,66,126,0} , 
{0,0,60,36,36,60,0,0} , 
{0,0,0,24,24,0,0,0} , 
{0,0,60,36,36,60,0,0} , 
{0,126,66,66,66,66,126,0} 
};


/*
byte sequence[][8] = {
{
0b11111111,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b11111111
} , 
{
0b00000000,
0b01111110,
0b01000010,
0b01000010,
0b01000010,
0b01000010,
0b01111110,
0b00000000
} , 
{
0b00000000,
0b00000000,
0b00111100,
0b00100100,
0b00100100,
0b00111100,
0b00000000,
0b00000000
} , 
{
0b00000000,
0b00000000,
0b00000000,
0b00011000,
0b00011000,
0b00000000,
0b00000000,
0b00000000
} , 
{
0b00000000,
0b00000000,
0b00111100,
0b00100100,
0b00100100,
0b00111100,
0b00000000,
0b00000000
} , 
{
0b00000000,
0b01111110,
0b01000010,
0b01000010,
0b01000010,
0b01000010,
0b01111110,
0b00000000
} 
};
*/
void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
}

void loop() {
  if (i == frames) {
    i = 0;
  }
  while (waitCount < waitCycles) {
    waitCount++;
    for (int j =0; j < 8; j++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin,MSBFIRST, 0); 
     // shiftOut(dataPin, clockPin,MSBFIRST, 255);
      digitalWrite(latchPin, HIGH);
      digitalWrite(latchPin, LOW);
      //shiftOut(dataPin, clockPin,MSBFIRST, sequence[i][j]); 
      shiftOut(dataPin, clockPin,MSBFIRST, columns[j]);
      digitalWrite(latchPin, HIGH);
    }
  }
  i++;
}

/*
void shiftOuts(int myDataPin, int myClockPin, byte myDataOut) {
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, LOW);
  digitalWrite(myClockPin, LOW);
  for (int k=7; k>=0; k--)  {
    digitalWrite(myClockPin, LOW);
    if (myDataOut & (1 << k)) {
      digitalWrite(myDataPin, HIGH);
    } else {  
      digitalWrite(myDataPin, LOW);
    }
    digitalWrite(myClockPin, HIGH);
    digitalWrite(myDataPin, LOW);
  }
  digitalWrite(myClockPin, LOW);
}
*/
