
//L298 Driver board, 2 DC motors, 2 PWM pins used

/* By LeRoy Miller, 2017
 *  This is a simple test, will spin the left motor forward
 *  for about 3 seconds, spin the right motor forward for
 *  about 3 seconds, spin the left wheel backward for 3 seconds
 *  and finally spin the right wheel backward for 3 seconds.
 *  
 *  PWM is not tested in this sketch but is used to drive the motors
 *  at full speed.
 *  
 *  Feel free to use and modify as needed.
 */

int left = 7; //IN1
int right = 8; //IN3
int leftpwm = 10; //IN2
int rightpwm = 11; //IN4

int setspeed = 255;

void setup() {
  // put your setup code here, to run once:

pinMode(left, OUTPUT);
pinMode(right, OUTPUT);
pinMode(leftpwm, OUTPUT);
pinMode(rightpwm, OUTPUT);
stop();
delay(3000);
forwardleft();
delay(3000);
stop();
delay(3000);
forwardright();
delay(3000);
stop();
delay(3000);
backleft();
delay(3000);
stop();
delay(3000);
backright();
delay(3000);
stop();

}

void loop() {
  // put your main code here, to run repeatedly:


}

void forwardleft() {
  digitalWrite(left, LOW);
  analogWrite(leftpwm, setspeed);
  
}

void backleft() {
  digitalWrite(left, HIGH);
  digitalWrite(leftpwm, 255 - setspeed);
  
}

void backright() {
  digitalWrite(right, HIGH);
  digitalWrite(rightpwm, 255 - setspeed);
}

void forwardright() {
  digitalWrite(right, LOW);
  analogWrite(rightpwm, setspeed);
  
}

void stop() {
  digitalWrite(right, LOW);
  digitalWrite(left, LOW);
  analogWrite(rightpwm, 0);
  analogWrite(leftpwm, 0);
}

