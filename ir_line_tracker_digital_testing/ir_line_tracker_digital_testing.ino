/*
* Verifies the IR Line tracking output
* feiticeir0@whatgeek.com.pt
* Free to share ! 
*/
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

//print the values
Serial.print (L);
Serial.print(",");
Serial.print (C);
Serial.print(",");
Serial.println (R);
}