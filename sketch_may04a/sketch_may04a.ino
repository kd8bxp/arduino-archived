

#define FORWARD 0 //set to 1 to test FORWARD/BACKWARD using switches

#define LEFTSWITCH 6 //D6 Blue Connector Marked "B"
#define RIGHTSWITCH 5 //D5 Blue Connector Marked "G"
#define M1SPD 10 //D10 PWM M1 speed pin
#define M2SPD 11 //D11 PWM M2 speed pin
#define M2DIR 13 //D13 PWM M2 direction
#define M1DIR 12 //D12 M1 direction
#define BUZZER 4 //D4

int lowspd = 100; //PWM for stall of motors

int setspd = 150; //This is PWM the motors will run at.

void setup() {
    
Serial.begin(9600);
pinMode(LEFTSWITCH, INPUT_PULLUP);
pinMode(RIGHTSWITCH, INPUT_PULLUP);
pinMode(M1SPD, OUTPUT);
pinMode(M2SPD, OUTPUT);
pinMode(M1DIR, OUTPUT);
pinMode(M2DIR, OUTPUT);
stop();
}

void loop() {
setSpd(150,150); Forward(); 
  if (digitalRead(LEFTSWITCH) == LOW) {
    Serial.println("Left");
       
    if (FORWARD) {setSpd(150,150); Forward(); } else {setSpd(150, 0); Left();}
    delay(1000);
    }
if (digitalRead(RIGHTSWITCH) == LOW) {
  Serial.println("Right");
  if (FORWARD) {Backward(150,150);} else {setSpd(0,150);Right();}
  delay(1000);
  }
//stop();
}

void stop() {
  analogWrite(M1SPD, 0); 
  digitalWrite(M1DIR, LOW); 
  analogWrite(M2SPD, 0);
  digitalWrite(M2DIR, LOW);
}

void setSpd(int m1, int m2) {
  if (m1 < lowspd) {m1 = lowspd;}
  if (m2 < lowspd) {m2 = lowspd;}
  if (m1 > 255) {m1 = 255;}
  if (m2 > 255) {m2 = 255;}
  analogWrite(M1SPD, m1);
  analogWrite(M2SPD, m2);
}
void Forward() {
  digitalWrite(M1DIR, LOW);
  digitalWrite(M2DIR, LOW);
  }

void Backward(int m1, int m2) {
  digitalWrite(M1DIR, HIGH);
  analogWrite(M1SPD, 255 - m1);
  digitalWrite(M2DIR, HIGH);
  analogWrite(M2SPD, 255 - m2);
}

void Left() {
  digitalWrite(M1DIR, HIGH);
  digitalWrite(M2DIR, LOW);
}

void Right() {
  digitalWrite(M1DIR, LOW);
  digitalWrite(M2DIR, HIGH);
}

