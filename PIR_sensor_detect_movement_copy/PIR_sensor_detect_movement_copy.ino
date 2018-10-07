// Uses a PIR sensor to detect movement, buzzes a buzzer
// more info here: http://blog.makezine.com/projects/pir-sensor-arduino-alarm/
// email me, John Park, at jp@jpixl.net
// based upon:
// PIR sensor tester by Limor Fried of Adafruit
// tone code by michael@thegrebs.com

 
int ledmotion = 13;                // choose the pin for the LED
int MotionPin = 4;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
 
void setup() {
  pinMode(ledmotion, OUTPUT);      // declare LED as output
  pinMode(MotionPin, INPUT);     // declare sensor as input
  Serial.begin(9600);
attachInterrupt(0,movement, HIGH);

}

void movement() {
	Serial.println("Motion Detected!");
}

void loop(){

  val = digitalRead(MotionPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledmotion, HIGH);  // turn LED ON
         
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      
    }
  } else {
      digitalWrite(ledmotion, LOW); // turn LED OFF
           if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }



}
  


