void switchResults() {
  switch (SwitchIN) {
    case 0: 
      Serial.println("STOP");
      updSender(speed, SwitchIN, mode);
      break;
     case 1:
     Serial.println("UP");
     updSender(speed, SwitchIN, mode);
     break;
     case 2:
     Serial.println("CENTER");
     switchMode();
     updSender(speed, SwitchIN, mode);
     break;
     case 3:
     Serial.println("LEFT");
     updSender(speed, SwitchIN, mode);
     break;
     case 4:
     Serial.println("DOWN");
     updSender(speed, SwitchIN, mode);
     break;
     case 5: 
     Serial.println("RIGHT");
     updSender(speed, SwitchIN, mode);
     break;
     default:
     SwitchIN = 0;
     updSender(speed, SwitchIN, mode);
     break;
  }
}

/*
 * 0 - STOP
 * 1 - UP
 * 2 - CENTER
 * 3 - LEFT
 * 4 - DOWN
 * 5 - RIGHT
 */

