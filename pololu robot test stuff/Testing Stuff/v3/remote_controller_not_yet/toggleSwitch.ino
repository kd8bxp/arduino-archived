/*
 * 0 - STOP
 * 1 - UP
 * 2 - CENTER
 * 3 - LEFT
 * 4 - DOWN
 * 5 - RIGHT
 */


void toggleSwitch() {
  if (digitalRead(UP) == 0) {SwitchIN = 1; return;}
  if (digitalRead(CENTER) == 0) {SwitchIN = 2; return;}
  if (digitalRead(LEFT) == 0) {SwitchIN = 3; return;}
  if (digitalRead(DOWN) == 0) {SwitchIN = 4; return;}
  if (digitalRead(RIGHT)  == 0) {SwitchIN = 5; return;}
  SwitchIN = 0; 
  return;
}

