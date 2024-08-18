
//Line follow appears to be analog sensors on the Hardware I2C line
// Port 3 - change pins if needed to move to different port
//Appears to be analog sensor

void setup() {
  pinMode(A4, INPUT); //left sensor
  pinMode(A5, INPUT); //right sensor
  Serial.begin(9600);
}

void loop() {
  int valL = analogRead(A4);
  int valR = analogRead(A5);
  Serial.print(valL); Serial.print(" "); Serial.println(valR);
  delay(600);
}
