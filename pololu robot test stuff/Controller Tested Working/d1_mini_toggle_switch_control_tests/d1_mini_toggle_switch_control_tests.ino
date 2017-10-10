/*
 * 0 - STOP
 * 1 - UP
 * 2 - CENTER
 * 3 - LEFT
 * 4 - DOWN
 * 5 - RIGHT
 */

#define UP D3
#define CENTER D4
#define LEFT D0
#define DOWN D5
#define RIGHT D6

int SwitchIN = 0;

void setup() {
  pinMode(UP, INPUT_PULLUP);
  pinMode(CENTER, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {
  toggleSwitch();
  switchResults();
delay(100);
}

void toggleSwitch() {
  if (digitalRead(UP) == 0) {SwitchIN = 1; return;}
  if (digitalRead(CENTER) == 0) {SwitchIN = 2; return;}
  if (digitalRead(LEFT) == 0) {SwitchIN = 3; return;}
  if (digitalRead(DOWN) == 0) {SwitchIN = 4; return;}
  if (digitalRead(RIGHT)  == 0) {SwitchIN = 5; return;}
  SwitchIN = 0; 
  return;
}

void switchResults() {
  switch (SwitchIN) {
    case 0: 
      Serial.println("STOP");
      break;
     case 1:
     Serial.println("UP");
     break;
     case 2:
     Serial.println("CENTER");
     break;
     case 3:
     Serial.println("LEFT");
     break;
     case 4:
     Serial.println("DOWN");
     break;
     case 5: 
     Serial.println("RIGHT");
     break;
     default:
     SwitchIN = 0;
     break;
  }
}

