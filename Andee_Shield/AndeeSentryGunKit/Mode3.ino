
void setMode3()
{
  Serial.println("  Set Mode 2 - Voice");

  HServo.attach(HPin);
  VServo.attach(VPin);
  Andee.changeScreen(4);
  delay(100);
  
  TitleBox.setUnit("Joystick Control");
 WatchTitle.setId(WATCH_TITLE);
  WatchTitle.setType(WATCH);
  WatchBack.setId(WATCH_BUTTON0);
  WatchBack.setType(WATCH);
  WatchButton1.setId(WATCH_BUTTON1);
  WatchButton1.setType(WATCH);
  WatchButton2.setId(WATCH_BUTTON2);
  WatchButton2.setType(WATCH);
  WatchButton3.setId(WATCH_BUTTON3);
  WatchButton3.setType(WATCH);
  WatchButton4.setId(WATCH_BUTTON4);
  WatchButton4.setType(WATCH);
  WatchTitle.setWatchTitle("Voice Command");
  WatchButton1.setWatchTitle("Speak");
  WatchButton1.setWatchTitleColor("FFFFFFFF");
  
   button_Reset.setId(7);  
  button_Reset.setType(BUTTON_IN); // Set object as a slider
  button_Reset.setLocation(3, 1, ONE_THIRD); // Sliders can only be full size
  button_Reset.setTitle("Back");
  button_Reset.setColor("FF8B8B83");
  button_Reset.requireAck(false);
  
  
   button_PC.remove();
   button_SentryControl.remove();
button_Up.remove();
    button_Left.remove();
    button_Right.remove();
    button_Down.remove();
    FireButton.remove();
    
    TitleBox.update();
    button_Reset.update();
  TitleBox.setUnit("Voice Control Mode");
}


void Mode3() {

  //Voice
  if (lastIsConnected == true) {

      if (button_Reset.isPressed2()) {
      Serial.println("Reset");

      HPos = 90;
      HServo.write(HPos);
      VPos = 0;
      VServo.write(VPos);
   //   Update = true;
    Mode = 0;
      Update = true;
    }
    if (WatchBack.isPressed2() == true) {
      Serial.println("back to mainpage");
      WatchBack.ack();
      Mode = 0;
      Update = true;
    }
    else if (WatchButton1.isPressed2() == true) {
      Serial.println("Activate Voice");
      WatchButton1.ack();
      Andee.textInput();

    }

    else if (WatchTitle.isPressed2()) {
      WatchTitle.ack();
      memset(userInput, 0x00, 32);
      WatchTitle.ack();
      WatchTitle.getKeyboardMessage(userInput);
      String command = String(userInput);

      Serial.print("command = ");

      Serial.println(userInput);
      command.toUpperCase();
     
    
      if (command.indexOf("LEFT") >= 0) {
        HPos = 135;
        HServo.write(HPos);
        delay(500);
      }
      if (command.indexOf("RIGHT") >= 0) {
        HPos = 45;
        HServo.write(HPos);
        delay(500);
      }
      if (command.indexOf("UP") >= 0) {
        VPos = 75;
        VServo.write(VPos);
        delay(500);
      }
      if (command.indexOf("DOWN") >= 0) {
        VPos = 10;
        VServo.write(VPos);
        delay(500);
      }
      if (command.indexOf("CENTER") >= 0 || command.indexOf("RESET") >= 0 || command.indexOf("MIDDLE") >= 0) {
        HPos = 90;
        HServo.write(HPos);
        VPos = 10;
        VServo.write(VPos);
        delay(500);
      }

      if (command.indexOf("FIRE") >= 0 || command.indexOf("SHOOT") >= 0) {
        Serial.println("shoot");
        digitalWrite(MissilePin, HIGH);
        delay(50);
        digitalWrite(MissilePin, LOW);
      }
      if (command.indexOf("LASER ON") >= 0) {
        laserState = HIGH;
        digitalWrite(LaserPin, laserState);
      }

      if (command.indexOf("LASER OFF") >= 0) {
        laserState = LOW;
        digitalWrite(LaserPin, laserState);
      }
    }

  }

}

