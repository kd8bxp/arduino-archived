
void setMode4()
{

  Serial.println("  Set Mode 4 - Crown Control");

  HServo.attach(HPin);
  VServo.attach(VPin);
  Andee.changeScreen(9);
  delay(100);
  
  TitleBox.setUnit("Crown Control");
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
  WatchTitle.setWatchTitle("Watch Control");
  WatchButton1.setWatchTitle(" ");
  WatchButton1.setWatchTitleColor("FFFFFFFF");
  WatchButton2.setWatchTitle(" ");
  WatchButton2.setWatchTitleColor("FFFFFFFF");
  WatchButton3.setWatchTitle(" ");
  WatchButton3.setWatchTitleColor("FFFFFFFF");
  WatchButton4.setWatchTitle(" ");
  WatchButton4.setWatchTitleColor("FFFFFFFF");
 
 
   button_PC.remove();
   button_SentryControl.remove();
 

  Serial.println("  Set Mode 2 End");
}

void Mode4() {
//turret control
  if (lastIsConnected == true) {
    if (WatchBack.isPressed2() == true) {
      Serial.println("back to mainpage");
      WatchBack.ack();
       Mode = 0;
      Update = true;
    }
     else if (WatchButton1.isPressed2() == true) {
 
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
      WatchButton1.ack();
    }
    else{
      
      memset(userInput, 0x00, 32);
      WatchTitle.getKeyboardMessage(userInput);
      String command = String(userInput);
      Serial.print("Command = ");
      Serial.print (command);
       if(command[0] =='H'){
         
       Serial.print("  Horizontal = ");
       int H_Angle = (int)(command[1]-32);
       int baseAngle = 90 +((7-H_Angle)*10);
      
       Serial.println(baseAngle);
       HServo.write( baseAngle);
       
       }
       else if(command[0] =='V'){
         
       Serial.print("  Vertical = ");
       int V_Angle = (int)(command[1]-32);
       
       Serial.println(V_Angle);
       VServo.write( V_Angle*10);
       
       }
    }
  }
}
