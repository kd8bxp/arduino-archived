
void serialParse() {
  if (stringComplete) {
    inputString.trim();
    call=inputString.substring(0,inputString.indexOf(" "));
    call.toUpperCase();
    msg=inputString.substring(inputString.indexOf(" ")+1);//,inputString.length()-1);
   
    
    inputCheck();
    if (pass1 == true && pass2 == true && pass3 == true) { Serial.println("Sending message"); txMsg();} else { Serial.println("Not sending..."); }
      
    //reset flags to false
    callPassCheck = false;
    msgPassCheck = false;
    pass1 = false;
    pass2 = false;
    pass3 = false;
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      serialParse();
    }
  }
}
