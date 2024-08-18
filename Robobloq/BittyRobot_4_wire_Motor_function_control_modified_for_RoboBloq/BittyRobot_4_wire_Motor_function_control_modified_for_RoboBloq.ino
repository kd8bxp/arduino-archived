/******************************************
 * 
 * This program contains a base set of sub routines for motor control.
 * the program it's self does nothing, and is used to show how to create sub-routines
 * This is using a standard 4 wire set, with 2 wires being digital (HIGH/LOW) and 2 
 * PWM pins for speed control.
 * 
 * Aug 2, 2017 - Moved motor control to sub routines. LeRoy Miller, 2017 RabbitRobots.com
 * Routines are:
 * left(speed, duration)
 * right(speed, duration)
 * forward(speed, duration)
 * backwards(speed, duration)
 * stop()
 * 
 * These are your 5 basic functions: What other functions could be added?
 * 
 ******************************************/

/*  Using just 4 wires, you can control 2 motors with speed and direction.
 *  Each motor needs a digital pin (HIGH/LOW) and a PWM pin.
 *  for direction control, inverted logic is used. See examples below.
 *  PWM for Speed Control and direction.  
 */
  // Needs a lot of testing for RoboBloq Robot
  
//The digital pins that the motor controller is attatched to on the UNO
const int L_PIN = 9; //Non-PWM Left wheel
const int L_PWM = 10; //PWM Left wheel (speed control)
const int R_PIN = 5; //Non-PWM Right wheel
const int R_PWM = 6; //PWM Right wheel (speed control)

void setup() { /*----( SETUP: RUNS ONCE )----*/

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_PIN, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PIN, OUTPUT);
  pinMode(R_PWM, OUTPUT);  
}

void loop() { 
  
  forward(255, 1000);
  stop();
  backwards(255, 1000);
  stop();
  while(1) {}
}


void left(int speed, int duration) {
    digitalWrite(R_PIN, LOW);
    analogWrite(R_PWM, speed);
    delay(duration);
}

void right(int speed, int duration) {
    digitalWrite(L_PIN, LOW); 
    analogWrite(L_PWM, speed);
    delay(duration);
}

void forward(int speed, int duration) {
    digitalWrite(L_PIN, LOW); //Set both the left and right motors to go forwards
    analogWrite(L_PWM, speed);
    digitalWrite(R_PIN, LOW);
    analogWrite(R_PWM, speed);
    delay(duration);
}

void backwards(int speed, int duration) {
    digitalWrite(L_PIN, HIGH); //Set both the left and right motors to go in reverse
    analogWrite(L_PWM, 255 - speed);
    digitalWrite(R_PIN, HIGH);
    analogWrite(R_PWM, 255 - speed);       
    delay(duration);
}

void stop() {
    digitalWrite(L_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_PWM, LOW);
    digitalWrite(R_PIN, LOW);
    digitalWrite(R_PWM, LOW);
        }

/* ( End ) */
