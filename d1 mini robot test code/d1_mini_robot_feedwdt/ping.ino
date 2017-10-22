void ping() {
long duration;// the Pulse time received;

  pinMode(ultrasonicPin, OUTPUT);
  digitalWrite(ultrasonicPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ultrasonicPin,LOW);
  pinMode(ultrasonicPin,INPUT);
  duration = pulseIn(ultrasonicPin,HIGH);
  
 
  RangeInInches = duration/74/2;

 //   Serial.println("The distance to obstacles in front is: ");
 // Serial.print(RangeInInches);//0~157 inches
/*if (RangeInInches <=39) {
  stop();
  cmd = 0;
  }*/
}

