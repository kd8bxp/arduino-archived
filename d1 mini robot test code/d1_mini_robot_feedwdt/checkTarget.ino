
void checkTarget() {
  int diff = targetCount - m1steps;
  if (diff <= 75) { M2.setmotor(_CCW, setSpeed % 20); }
  //if (diff <= 30) { M2.setmotor(_CCW, setSpeed % 15); } 
  if (diff <= 2 ) { M2.setmotor(_SHORT_BRAKE); M2.setmotor(_STOP); /*while(1) {ESP.wdtFeed();}*/}
  
}


/* the above is still the test code.... it needs to be updated to work with two motors 
 *  and to use the motor functions.
 */

 void setTargetM1(int tCounter) {
    targetCountM1 = tCounter * rotationCount; //this is how many rotations for M1
 }

 void setTargetM2(int tCounter) {
  targetCountM2 = tCounter * rotationCount; //this is how many roations for M2
 }

 void setTarget(int tCounter) {
  targetCount = tCounter *rotationCount; //both motors
 }

// Motor Functions for using encoders

void M1Forward() {
  isRunning = 1;
}

void M1Backward() {
  isRunning = 1;
}

void M2Forward() {
  isRunning = 1;
}

void M2Backward() {
  isRunning = 1;
}

/*
 * useage set a target for one or both motors
 * if both motors normal functions can be used
 * 
 * setTarget(10); //this is how many full rotations of the wheel you would like
 * forward(100);
 * while(isRunning()) {
 * checkTarget();
 * }
 * 
 */

void setDegree(int tDegree) {
  //maths to get to degree
  
}

