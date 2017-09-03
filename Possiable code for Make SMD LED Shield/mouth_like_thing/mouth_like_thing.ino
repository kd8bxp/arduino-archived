int clockPin = 12;
int latchPin = 8;
int dataPin = 11;

int del;

byte ledStates[8] = {B00000000, B00011000, B00111100, B01111110, B11111111, B11111111, B11111111, B01100110};
byte GroundLEDs [8] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111110};


void setup() {
 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, B11111111);  
    digitalWrite(latchPin, HIGH);
    
    Serial.begin(9600);
}

void loop() {
  
  for(int i=0;i<8;i++){
    
  SetStates(ledStates[i]);
  //GroundCorrectLED (GroundLEDs[i]);
  
del = 34;
delay(del);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, B11111111);  
    digitalWrite(latchPin, HIGH);
    
delay(del);
}}

void GroundCorrectLED (byte states){
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, states);  
    digitalWrite(latchPin, HIGH);
    
}

void SetStates (byte statesx){
       
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, statesx);  
    digitalWrite(latchPin, HIGH);
    
     }
     

