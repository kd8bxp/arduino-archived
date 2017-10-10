void startTimer(int stopTime) {
 
  motorTimer = millis();
  motorStopTime = stopTime;

}

void updateMotor() {

   motorCurrentTimer = millis();

if (motorCurrentTimer - motorTimer >= motorStopTime) {
  stop();
  m1count = 0;
  m2count = 0;
  m1dir = 0;
  m2dir = 0;
  //motorTimer = motorCurrentTimer;
  }
  
}

/*
 * use startTimer to set how long you want the motor to run IE:
 * 
 * forward(100);
 * startTimer(1000); //one second
 * while (isRunning) {
 * update();  //or updateMotor();
 * }
 *  
 */
