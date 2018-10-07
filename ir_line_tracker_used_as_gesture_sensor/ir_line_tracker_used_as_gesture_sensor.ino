int L;
int C;
int R;

int freezetime1;
int freezetime2;
int freezetime3;

void setup() {

Serial.begin (9600);
pinMode (10,INPUT);
pinMode (9,INPUT);
pinMode (8,INPUT);


}
void loop() {

//read the input


//while (digitalRead(10) == 0 || digitalRead(9) == 0 || digitalRead(8) == 0) {}

L = digitalRead(10); //left sensor
C = digitalRead(9); //center sensor
R = digitalRead(8); // right sensor

if (L == 1) {
	freezetime1 = millis();
}

if (C == 1) {
	freezetime2 = millis();
}

if (R == 1) {
	freezetime3 = millis();
}

if (freezetime1 < freezetime2 && freezetime2 < freezetime3) {
	Serial.println("Moving left to right");
	freezetime1 = 0;
	freezetime2 = 0;
	freezetime3 = 0;
}

if (freezetime1 > freezetime2 && freezetime2 > freezetime3) {
	Serial.println("Moving right to left");
	freezetime1 = 0;
	freezetime2 = 0;
	freezetime3 = 0;
}

}