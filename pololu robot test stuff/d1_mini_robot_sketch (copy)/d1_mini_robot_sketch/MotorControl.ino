void right(int pwm) {
  //Turn right Left Motor Forward, Right Motor Backward
  pwm = checkPWM(pwm);
  M1.setmotor( _CCW, pwm);
  M2.setmotor(_CW, pwm);
  isRunning = 1;
  yield();
}

void left(int pwm) {
  //Turn left Right Motor Forward, Left Motor Backward
  pwm = checkPWM(pwm);
  M1.setmotor( _CW, pwm);
  M2.setmotor(_CCW, pwm);
  isRunning = 1;
  yield();
}

void forward(int pwm) {
  //M1 _CCW M2_CCW
  pwm = checkPWM(pwm);
  M1.setmotor(_CCW, pwm);
  M2.setmotor(_CCW, pwm);
  isRunning = 1;
  yield();
}

void backward(int pwm) {
  //M1 _CW M2 _CW
  pwm = checkPWM(pwm);
  M1.setmotor(_CW, pwm);
  M2.setmotor(_CW, pwm);
  isRunning = 1;
  yield();
}

void stop() {
  M1.setmotor(_STOP);
  M2.setmotor(_STOP);
  encoderReset();
  yield();
}

void mbreak() {
  M1.setmotor(_SHORT_BRAKE);
  M2.setmotor(_SHORT_BRAKE);
  encoderReset();
  yield();
}

void standby() {
  M1.setmotor(_STANDBY);
  M2.setmotor(_STANDBY);
  encoderReset();
  yield();
}

void encoderReset() {
  isRunning = 0;
  m1dir = 0;
  m2dir = 0;
  m1steps = 0;
  m2steps = 0;
}

void update() {
  updateMotor();
  yield();
}

int checkPWM(int pwm) {
  yield();
  return constrain(pwm, lowSpeed, maxSpeed);
  }

