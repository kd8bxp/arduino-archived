void resetEncoder() {
  encoderCurrentTimer = millis();

if (encoderCurrentTimer - encoderTimer >= 500) {
  m1steps = 0;
  m2steps = 0;
  encoderTimer = encoderCurrentTimer;
 }
  
}

