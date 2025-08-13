#define dirA 12
#define pwmA 3 
#define dirB 13 
#define pwmB 11
#define brkA 9 
#define brkB 8 


void setup() {
  pinMode(dirA, OUTPUT);
pinMode(dirB, OUTPUT);  
pinMode(pwmA, OUTPUT);
pinMode(pwmB, OUTPUT);
  pinMode(brkA, OUTPUT);
pinMode(brkB, OUTPUT);
digitalWrite(brkA, LOW);
digitalWrite(brkB, LOW);
}

void loop() {
 forward(0,0);
 delay(500);
 stop();
 delay(500);
 back(255,255);
 delay(500);
 stop();
 delay(500);

}

void stop() {
  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);
  digitalWrite(dirA, 0);
  digitalWrite(dirB, 0);
}

void forward(int lspd, int rspd) {
  analogWrite(pwmA, 255 - lspd);
  digitalWrite(dirA, HIGH);
  analogWrite(pwmB, 255 - rspd);
  digitalWrite(dirB, HIGH);
}

void back(int lspd, int rspd) {
  analogWrite(pwmA, lspd);
  digitalWrite(dirA, LOW);
  analogWrite(pwmB, rspd);
  digitalWrite(dirB, LOW);
}
