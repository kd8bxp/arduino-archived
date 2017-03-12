
void setMode2()
{

  Serial.println("  Set Mode 2 - Manual");

  HServo.attach(HPin);
  VServo.attach(VPin);
  Andee.changeScreen(8);
  delay(100);
  TitleBox.setUnit("Manual Mode");
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
  WatchTitle.setWatchTitle("Manual Control");
  WatchButton1.setWatchTitle(" ");
  WatchButton1.setWatchTitleColor("FFFFFFFF");
  WatchButton2.setWatchTitle(" ");
  WatchButton2.setWatchTitleColor("FFFFFFFF");
  WatchButton3.setWatchTitle(" ");
  WatchButton3.setWatchTitleColor("FFFFFFFF");
  WatchButton4.setWatchTitle(" ");
  WatchButton4.setWatchTitleColor("FFFFFFFF");

  TitleBox.setUnit("Manual Mode");
  
  Blank.setId(4);  
  Blank.setType(DATA_OUT); // Set object as a slider
  Blank.setLocation(1, 1, ONE_THIRD); // Sliders can only be full size
  Blank.setTitle("UP");
 Blank.setColor("00000000");
 Blank.setTitleColor("00000000");
 Blank.setTitle(" ");
 Blank.setData(" ");
  
  button_Up.setId(5);  
  button_Up.setType(CIRCLE_BUTTON); // Set object as a slider
  button_Up.setLocation(1, 2,  ONE_THIRD); // Sliders can only be full size
  button_Up.setTitle("Up");
  button_Up.setColor("FF000000");
  button_Up.requireAck(false);
  
  button_Left.setId(6);  
  button_Left.setType(CIRCLE_BUTTON); // Set object as a slider
  button_Left.setLocation(2, 1, ONE_THIRD); // Sliders can only be full size
  button_Left.setTitle("Left");
  button_Left.setColor("FF000000");
  button_Left.requireAck(false);
  
  button_Reset.setId(7);  
  button_Reset.setType(BUTTON_IN); // Set object as a slider
  button_Reset.setLocation(3, 1, ONE_THIRD); // Sliders can only be full size
  button_Reset.setTitle("Back");
  button_Reset.setColor("FF8B8B83");
  button_Reset.requireAck(false);
  
  button_Right.setId(8);  
  button_Right.setType(CIRCLE_BUTTON); // Set object as a slider
  button_Right.setLocation(2, 3, ONE_THIRD); // Sliders can only be full size
  button_Right.setTitle("Right");
  button_Right.setColor("FF000000");
  button_Right.requireAck(false);
  
  button_Down.setId(9);  
  button_Down.setType(CIRCLE_BUTTON); // Set object as a slider
  button_Down.setLocation(3, 1, ONE_THIRD); // Sliders can only be full size
  button_Down.setTitle("Down");
  button_Down.setColor("FF000000");
  button_Down.requireAck(false);
 
 
   // Draw position slider
  FireButton.setId(10);  
  FireButton.setType(CIRCLE_BUTTON); // Set object as a slider
  FireButton.setLocation(2, 2, ONE_THIRD); // Sliders can only be full size
  FireButton.setTitle("Fire!");
  FireButton.setColor("FFFF0000");
  FireButton.requireAck(false);
  delay(100);
   
  delay(100);
   button_SentryControl.remove();
   button_SentryControl.remove();
   button_PC.remove();
    TitleBox.update();
   Blank.update();
   button_Up.update();
    button_Left.update();
  //  button_Reset.update();
    button_Right.update();
    button_Down.update();
    FireButton.update();
button_Reset.update();
  Serial.println("  Set Mode 2 End");
}

void Mode2() {

  //Manual
  if (lastIsConnected == true) {
    
    
    
    
    

    if (WatchBack.isPressed2() == true) {
      Serial.println("back to mainpage");
      WatchBack.ack();
       Mode = 0;
      Update = true;
    }
    else{
      
      int UpCount = max(WatchButton1.pressCounter(), button_Up.pressCounter());
       int DownCount = max(WatchButton2.pressCounter(), button_Down.pressCounter());
        int LeftCount = max(WatchButton3.pressCounter(), button_Left.pressCounter());
         int RightCount = max(WatchButton4.pressCounter(), button_Right.pressCounter());
    if (UpCount>0) {

      Serial.println("Up");

      VPos = min(VPos + (Sensitivity*UpCount), 110);
      VServo.write(VPos);
      WatchButton1.ack();

    }
    if (DownCount>0) {
      Serial.println("Down");

      VPos = max(VPos - (Sensitivity*DownCount), 0);
      VServo.write(VPos);
      WatchButton2.ack();

    }
    if (LeftCount>0) {
      Serial.println("Left");
      HPos = min(HPos + (Sensitivity*LeftCount), 170);
      HServo.write(HPos);
      WatchButton3.ack();
    }
    if (RightCount>0) {
      Serial.println("Right");

      HPos = max(HPos - (Sensitivity*RightCount), 10);
      HServo.write(HPos);
      WatchButton4.ack();
    }
    if (button_Reset.isPressed2()) {
      Serial.println("Reset");

      HPos = 90;
      HServo.write(HPos);
      VPos = 0;
      VServo.write(VPos); 
        Mode = 0;
      Update = true;
     // Update = true;
    }
    if (WatchTitle.isPressed2()|| FireButton.isPressed2()) {
      WatchTitle.ack();
      Serial.println("Fire");
      digitalWrite(MissilePin, HIGH);
      laserState = HIGH;
      digitalWrite(LaserPin, laserState);
      delay(50);
      digitalWrite(MissilePin, LOW);

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
    }
    }
  }

}
