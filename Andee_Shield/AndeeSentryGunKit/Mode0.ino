
void setMode0()
{

  Serial.println("  Set Mode 0 - Main Menu");
  Andee.changeScreen(2);
  delay(100);
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
  WatchTitle.setWatchTitle("Andee Turret");
  WatchButton1.setWatchTitle("Sentry");
  WatchButton1.setWatchTitleColor("FFAA0000");
  WatchButton2.setWatchTitle("Manual");
  WatchButton2.setWatchTitleColor("FFFF7F00");
  WatchButton3.setWatchTitle("Crown");
  WatchButton3.setWatchTitleColor("FFFF00FF");
  TitleBox.setUnit("Main Menu");

  button_Reset.setId(7);
  button_Reset.setType(BUTTON_IN); // Set object as a slider
  button_Reset.setLocation(3, 1, ONE_THIRD); // Sliders can only be full size
  button_Reset.setTitle("Reset");
  button_Reset.setColor("FF8B8B83");
  button_Reset.requireAck(false);
  delay(100);
  button_PC.setId(17);
  button_PC.setType(BUTTON_IN); // Set object as a slider
  button_PC.setLocation(1, 2, FULL); // Sliders can only be full size
  button_PC.setTitle("Manual Control");
  button_PC.setColor("FF008B45");
  //button_PC.requireAck(true);
  delay(100);
  button_SentryControl.setId(16);
  button_SentryControl.setType(BUTTON_IN); // Set object as a slider
  button_SentryControl.setLocation(2, 2, FULL); // Sliders can only be full size
  button_SentryControl.setTitle("Sentry Mode");
  button_SentryControl.setColor("FF00C5CD");
  //button_SentryControl.requireAck(true);
  Blank.remove();
  button_Up.remove();
  button_Left.remove();
  button_Right.remove();
  button_Down.remove();
  FireButton.remove();
  TitleBox.update();
  button_PC.update();
  button_SentryControl.update();
  button_Reset.update();

  Serial.println("  Set Mode 1 End");
}
int CurH = 90;
int CurV = 0;
int delta = 2;
int centerCounter = 0;
int pauseCounter = 0;
int sentryPassiveMode = 0;
int shootcounter;
void sentryPassive() {
  if (sentryPassiveMode == 0) {
    CurH = CurH + delta;
    if (CurH == 130 || CurH == 50) {
      //centerCounter++;
      delta = -delta;
    }

    HServo.write(CurH);
    VServo.write(0);
shootcounter++;
    if(random (1,200)==10){
      if(shootcounter>200){
        shootcounter=0;
      sentryPassiveMode = 1;
      }
      
    }
   
  }
  else if (sentryPassiveMode == 1) {
    
      delay(random(100,200));
      HServo.write(random(30,150));
      VServo.write(random(0,45));
      delay(300);
      for(int x =0;x<10;x++){
      digitalWrite(LaserPin, HIGH);
      delay(20);
      digitalWrite(LaserPin, LOW);
      delay(20);
      }
      
      delay(500);
      HServo.write(CurH);
      VServo.write(0);
      delay(250);
      sentryPassiveMode = 2;
  }
  else if (sentryPassiveMode == 2) {
    HServo.detach();
    VServo.detach();
    pauseCounter++;
    if (pauseCounter == 20) {
      pauseCounter=0;
      sentryPassiveMode = 0;
      
    HServo.attach(HPin);
    VServo.attach(VPin);

    }
  }
  
}
void Mode0() {
  //main menu

  Serial.println("  Mode 0 Starts");
  // HServo.detach();
  // VServo.detach();
  Serial.println("  1");
  Serial.println(lastIsConnected);
  if (lastIsConnected == true) {

    if (WatchBack.isPressed2() == true) {
      Serial.println("back to mainpage");
      WatchBack.ack();
    }
    else if (WatchButton1.isPressed2() == true) {

      Serial.println("To Mode 1 - Sentry");
      WatchButton1.ack();

      Mode = 1;
      Update = true;

    }
    else if (WatchButton2.isPressed2() == true) {
      Serial.println("To Mode 2 - Manual");

      WatchButton2.ack();

      Mode = 2;
      Update = true;

    }
    else if (WatchButton3.isPressed2() == true) {
      Serial.println("To Mode 3 - Voice");
      WatchButton3.ack();

      Mode = 4;
      Update = true;
    }

  }
  else{
  
  sentryPassive();
  }

  Serial.println("  2");
  if (button_Reset.isPressed2()) {
    Serial.println("Reset");

    HServo.attach(HPin);
    VServo.attach(VPin);
    HPos = 90;
    HServo.write(HPos);
    VPos = 0;
    VServo.write(VPos);
    WatchButton4.ack();
    Update = true;
  }

  Serial.println("  3");
  if ( button_PC.isPressed2()) {
    button_PC.ack();
    Mode = 2;
    Update = true;
  }

  Serial.println("  4");
  if ( button_SentryControl.isPressed2()) {
    button_SentryControl.ack();
    Mode = 1;
    Update = true;
  }

  Serial.println("  5");
  delay(10);

}
int ignorecounter = 0;

