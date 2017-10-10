void ping() {

  ultrasonic.DistanceMeasure();// get the current signal time;
  RangeInInches = ultrasonic.microsecondsToInches();//convert the time to inches;

 //   Serial.println("The distance to obstacles in front is: ");
 // Serial.print(RangeInInches);//0~157 inches

}

