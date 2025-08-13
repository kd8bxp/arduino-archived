//Use L9110 motor driver

#define armPinA 2 
#define armPinB 3 

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
pinMode(armPinA, OUTPUT);
pinMode(armPinB, OUTPUT);
    armStop();
    delay(1000);   
  }

void loop(){ 
close();
//delay(1000);
armStop();
delay(1000);
open();
//delay(1000);
armStop();
while(1);
}

void armStop() {
  //analogWrite(armPinB, 0);
  digitalWrite(armPinB, 0);
  digitalWrite(armPinA, 0);
  
}

void close() {
  //analogWrite(lpwm, 50);
  digitalWrite(armPinB, HIGH);
  digitalWrite(armPinA, LOW);
  delay(2500);
}

void open() {
  //analogWrite(lpwm, 25);
  digitalWrite(armPinB, LOW);
  digitalWrite(armPinA, HIGH);
  delay(2500);
}


