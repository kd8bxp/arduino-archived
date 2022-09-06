
#include "esp32-hal-ledc.h"

#define ldir D3 //26
#define lpwm D4 //27
#define rdir D6 /10
#define rpwm D5 //9

const int freq = 100, resolution = 8;

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
pinMode(ldir, OUTPUT);
pinMode(rdir, OUTPUT);  
pinMode(lpwm, OUTPUT);
pinMode(rpwm, OUTPUT);
  ledcSetup(0, freq, resolution); //left channel
  ledcSetup(2, freq, resolution); //right channel
  ledcAttachPin(lpwm, 0);
  ledcAttachPin(rpwm, 2);
    stop();   
  }

void loop(){ 
forward();
delay(1000);
stop();
back();
delay(1000);
stop();
while(1);
}

void stop() {
  ledcWrite(0, 0);
  ledcWrite(2, 0);
  digitalWrite(ldir, 0);
  digitalWrite(rdir, 0);
}

void forward() {
  ledcWrite(0, 120);
  digitalWrite(ldir, 1);
  ledcWrite(2, 120);
  digitalWrite(rdir, 1);
}

void back() {
  ledcWrite(0, 165);
  digitalWrite(ldir, LOW);
  ledcWrite(2, 165);
  digitalWrite(rdir, 0);
}


