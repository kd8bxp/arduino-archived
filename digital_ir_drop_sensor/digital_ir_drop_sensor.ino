int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value

void setup()
{
	Serial.begin(57600);
  pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output
  pinMode(inPin, INPUT);      // sets the digital pin 7 as input

//attachInterrupt(0, gohigh, RISING);
//attachInterrupt(0, golow, FALLING);

}

void loop()
{
  val = digitalRead(inPin);   // read the input pin
  digitalWrite(ledPin, val);   // sets the LED to the button's value
//Serial.print(val);

if (val == 0) {golow();}
if (val == 1) {gohigh();}
//if the sensor is too close to the floor it will trigger a drop (high)
//if it's too far away from the floor it will trigger a drop (high) 
//this can be adjusted with the set potentiometer
//a (HIGH) or (1) means that going to be a drop and should be on 
//an intrupt pin (green light on)
//a (LOW) or (0) means that it's still on a table or whatever (no drop)

//after some additional testing I think this is backward, High means it's still on the table, Low means it is going to fall
//after even more testing, an interrupt might not be the way to do this after all.
//after even more more testing - it seems I was right HIGH is a drop, LOW is on table (GREEN LED ON WITH A LOW!!!)

}

void gohigh() {
	Serial.print("HIGH!");
	delay(500);
}

void golow() {
	Serial.print("Low");
	delay(500);
}