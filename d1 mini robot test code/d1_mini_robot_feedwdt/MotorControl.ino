void right(int pwm) {
  //Turn right Left Motor Forward, Right Motor Backward
  pwm = checkPWM(pwm);
  M1.setmotor( _CCW, pwm);
  M2.setmotor(_CW, pwm);
  isRunning = 1;
  ESP.wdtFeed();
}

void left(int pwm) {
  //Turn left Right Motor Forward, Left Motor Backward
  pwm = checkPWM(pwm);
  M1.setmotor( _CW, pwm);
  M2.setmotor(_CCW, pwm);
  isRunning = 1;
  ESP.wdtFeed();
}

void forward(int pwm) {
  //M1 _CCW M2_CCW
  pwm = checkPWM(pwm);
  M1.setmotor(_CCW, pwm);
  M2.setmotor(_CCW, pwm);
  isRunning = 1;
  ESP.wdtFeed();
}

void backward(int pwm) {
  //M1 _CW M2 _CW
  pwm = checkPWM(pwm);
  M1.setmotor(_CW, pwm);
  M2.setmotor(_CW, pwm);
  isRunning = 1;
  ESP.wdtFeed();
}

void stop() {
  M1.setmotor(_STOP);
  M2.setmotor(_STOP);
  encoderReset();
  ESP.wdtFeed();
}

void mbreak() {
  M1.setmotor(_SHORT_BRAKE);
  M2.setmotor(_SHORT_BRAKE);
  encoderReset();
  ESP.wdtFeed();
}

void standby() {
  M1.setmotor(_STANDBY);
  M2.setmotor(_STANDBY);
  encoderReset();
  ESP.wdtFeed();
}

void encoderReset() {
  isRunning = 0;
  m1dir = 0;
  m2dir = 0;
  m1steps = 0;
  m2steps = 0;
  cmd = 0;
}

void update() {
  updateMotor();
  ESP.wdtFeed();
}

int checkPWM(int pwm) {
  ESP.wdtFeed();
  return constrain(pwm, lowSpeed, maxSpeed);
  }

