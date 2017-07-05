
float totalInches = 9.0243;
int feet;
int inches;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

totalInches = round(totalInches);
feet = (totalInches)/12;
inches = int(totalInches) % 12;

Serial.print(feet);
Serial.print(" ft. ");
Serial.print(inches);
Serial.print(" in. ");

}

void loop() {
  // put your main code here, to run repeatedly:

}
