/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
 // while (!Serial); //if just the the basic function, must connect to a computer
  Serial.println("Serial Input test");
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    inputString.trim();
    //if (inputString.indexOf(" ") == 0) { inputString.trim(); }
    String call=inputString.substring(0,inputString.indexOf(" "));
    call.toUpperCase();
    String msg=inputString.substring(inputString.indexOf(" ")+1);//,inputString.length()-1);
    //char buf[inputString.length()+1];
    //inputString.toCharArray(buf, inputString.length()+1);
    
//    sscanf(buf,"%5s %s", call, msg);
    Serial.println(call);
    Serial.println(msg);
    //Serial.println(call+msg);
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
 
  serialEvent();
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
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
    }
  }
}
