
void setMode1()
{
  Serial.println("  Set Mode 1 - Sentry");

  HServo.attach(HPin);
  VServo.attach(VPin);
  Andee.changeScreen(2);
  delay(100);
  
  TitleBox.setUnit("Sentry Mode");
  WatchTitle.setId(WATCH_TITLE);
  WatchTitle.setType(WATCH_TITLE);
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
  WatchTitle.setWatchTitle("Sentry");
  WatchButton1.setWatchTitle("Laser");
  WatchButton1.setWatchTitleColor("FFFF0000");
  WatchButton2.setWatchTitle("Fire");
  WatchButton2.setWatchTitleColor("FF00CA00");
  WatchButton3.setWatchTitle("Rapid Fire");
  WatchButton3.setWatchTitleColor("FFFF8000");
  laserState = LOW;
  digitalWrite(LaserPin, laserState);
  
   button_Reset.setId(7);  
  button_Reset.setType(BUTTON_IN); // Set object as a slider
  button_Reset.setLocation(3, 1, ONE_THIRD); // Sliders can only be full size
  button_Reset.setTitle("Back");
  button_Reset.setColor("FF8B8B83");
  button_Reset.requireAck(false);
  FireButton.setId(10);  
  FireButton.setType(CIRCLE_BUTTON); // Set object as a slider
  FireButton.setLocation(2, 2, FULL); // Sliders can only be full size
  FireButton.setTitle("Fire!");
  FireButton.setColor("FFFF0000");
  FireButton.requireAck(false);
  
  button_Up.remove();
    button_Left.remove();  
    button_Right.remove();
    button_Down.remove();
    FireButton.remove();
   button_PC.remove();
   button_SentryControl.remove();
    TitleBox.update();
    
   FireButton.update();
    button_Reset.update();
  TitleBox.setUnit("Sentry Mode");
}

void Mode1() {
  VPos = 10;
  VServo.write(VPos);
  if (ignorecounter > 0) {
    ignorecounter--;
  }
  else {
    if (Clockwise == true) {
      HPos = min(HPos + change, 170);
    }
    else {
      HPos = max(HPos - change, 10);
    }


    HServo.write(HPos);
    if (HPos == 170 || HPos == 10) {
      Clockwise = !Clockwise;
    }
  }
  //Sentry
  if (lastIsConnected == true) {
    if (WatchBack.isPressed2() == true) {
      Serial.println("back to mainpage");
      WatchBack.ack();
      Mode = 0;
      Update = true;
    }
    else if (WatchButton1.isPressed2() == true) {

      Serial.println("Activate Laser");

      laserState = !laserState;
      digitalWrite(LaserPin, laserState);
      if (laserState == HIGH) {
        WatchButton1.setWatchTitleColor("FF00FF00");
      }
      else  if (laserState == HIGH) {
        WatchButton1.setWatchTitleColor("FFFF0000");
      }
      WatchButton1.ack();
    }
    else if (WatchButton2.isPressed2() == true) {
      Serial.println("Fire");
      WatchButton2.ack();
      digitalWrite(MissilePin, HIGH);
      delay(50);
      digitalWrite(MissilePin, LOW);
      delay(500);
      ignorecounter = 2;
    }
    else if (WatchButton3.isPressed2() == true) {
      Serial.println("Shoot All");
      WatchButton3.ack();
      digitalWrite(MissilePin, HIGH);
      delay(1000);
      digitalWrite(MissilePin, LOW);
      delay(1000);
      ignorecounter = 2;
    }
    if(FireButton.isPressed2() == true){
      
      digitalWrite(MissilePin, HIGH);
      digitalWrite(LaserPin, HIGH);
      delay(50);
      
      digitalWrite(MissilePin, LOW);
      digitalWrite(LaserPin, LOW);
      delay(50);
      digitalWrite(LaserPin, HIGH);
      delay(50);
      digitalWrite(LaserPin, LOW);
      delay(50);
      digitalWrite(LaserPin, HIGH);
      delay(50);
      digitalWrite(LaserPin, LOW);
      delay(50);
      digitalWrite(LaserPin, HIGH);
      delay(50);
      digitalWrite(LaserPin, LOW);
      delay(50);
      digitalWrite(LaserPin, HIGH);
      delay(50);
      digitalWrite(LaserPin, LOW);
      delay(50);
      digitalWrite(LaserPin, HIGH);
      delay(50);
      digitalWrite(LaserPin, LOW);
      delay(50);
      digitalWrite(LaserPin, HIGH);
      delay(50);
      digitalWrite(LaserPin, LOW);
      delay(50);
      
      digitalWrite(LaserPin, laserState);
    }
    
    if (button_Reset.isPressed2()) {
      Serial.println("Reset");

      HPos = 90;
      HServo.write(HPos);
      VPos = 0;
      VServo.write(VPos);
//Update = true;
      delay(1500);
        Mode = 0;
      Update = true;
    }

  }
}

