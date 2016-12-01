const byte laserPin = 17;
const byte interruptPin = 18;
volatile byte state = LOW;

void setup() {
  pinMode(laserPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  //attachInterrupt(interruptPin, firelaser, LOW);
  attachInterrupt(5, firelaser, LOW);
}

void loop() {
  
}

void firelaser() {
	digitalWrite(laserPin, state);
  state = !state;
}