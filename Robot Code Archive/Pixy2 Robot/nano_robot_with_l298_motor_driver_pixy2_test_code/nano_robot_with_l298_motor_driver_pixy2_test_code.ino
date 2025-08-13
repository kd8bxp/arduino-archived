
#define ldir 4 
#define lpwm 5 //10 
#define rdir 7 //5 
#define rpwm 6 //9

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
pinMode(ldir, OUTPUT);
pinMode(rdir, OUTPUT);  
pinMode(lpwm, OUTPUT);
pinMode(rpwm, OUTPUT);
  
    stop();
    delay(1000);   
  }

void loop(){ 
forward(75,75); //left, right PWM values
delay(1000);
stop();
delay(1000);
back(75,75);
delay(1000);
stop();
while(1);
}

void stop() {
  analogWrite(lpwm, 0);
  analogWrite(rpwm, 0);
  digitalWrite(ldir, 0);
  digitalWrite(rdir, 0);
}

void forward(int lspd, int rspd) {
  analogWrite(lpwm, 255 - lspd);
  digitalWrite(ldir, HIGH);
  analogWrite(rpwm, 255 - rspd);
  digitalWrite(rdir, HIGH);
}

void back(int lspd, int rspd) {
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, LOW);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, LOW);
}


