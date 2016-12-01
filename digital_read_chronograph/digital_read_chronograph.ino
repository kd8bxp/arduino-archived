const int SPACING = 44; //22;

void setup() {

 Serial.begin (9600);
pinMode (10,INPUT);
pinMode (9,INPUT);
pinMode (8,INPUT);
}
void loop() {
//read the input
int L = digitalRead(10); //left sensor
int C = digitalRead(9); //center sensor
int R = digitalRead(8); // right sensor



while (digitalRead(10) < 1) {
	
}
Serial.println("Start");

int t1 = micros();
//int t1 = millis();

while (digitalRead(8) < 1) {
	
}


int t2 = micros();
//int t2 = millis();

Serial.print("Time in Microseconds: " );
Serial.println(t2-t1);

float spd = (SPACING * 1000.0) / (t2 - t1);

Serial.print("Speed: ");
Serial.print(spd);
Serial.println(" meter/sec");

Serial.print(spd * 2.236936);
Serial.println(" miles/hour");

Serial.println("Stop!");
}