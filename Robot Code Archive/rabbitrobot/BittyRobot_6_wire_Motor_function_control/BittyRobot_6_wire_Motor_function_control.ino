/******************************************
 * 
 * This program contains a base set of sub routines for motor control.
 * the program it's self does nothing, and is used to show how to create sub-routines
 * This is using a standard 6 wire set, with 4 wires being digital (HIGH/LOW) and 2 enable
 * pins with speed control using PWM.
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

/* Enable Pins are using PWM for Speed Control, they also "enable" the motor to move, 
 * The other pins are used for direction control. 
 * Using this method, it takes 6 wires to control two motor with speed and direction control
 */
 
const int L_ENABLE_PIN = 7; //The digital pins that the motor controller is attatched to one the MEGA
const int L_FORWARDS_PIN = 6;
const int L_REVERSE_PIN = 5;
const int R_FORWARDS_PIN = 4;
const int R_REVERSE_PIN = 3;
const int R_ENABLE_PIN = 2;

void setup() { /*----( SETUP: RUNS ONCE )----*/

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_ENABLE_PIN, OUTPUT);
  pinMode(L_FORWARDS_PIN, OUTPUT);
  pinMode(L_REVERSE_PIN, OUTPUT);
  pinMode(R_FORWARDS_PIN, OUTPUT);
  pinMode(R_REVERSE_PIN, OUTPUT);
  pinMode(R_ENABLE_PIN, OUTPUT);
  
}

void loop() { 
  forward(60, 1000);
  stop();
  while(1) {}
}

void forward(int speed, int duration) {
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set the left motor to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, LOW); //Set the right motor to go in reverse
    digitalWrite(R_REVERSE_PIN, HIGH);
    analogWrite(L_ENABLE_PIN, speed); //Make both motors run at spinning driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    delay(duration); //Don't do anything else until randomly-generated time has elapsed

}

void backwards(int speed, int duration) {
    digitalWrite(L_FORWARDS_PIN, LOW); //Set the left motor to in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    digitalWrite(R_FORWARDS_PIN, HIGH); //Set the right motor to go forwards
    digitalWrite(R_REVERSE_PIN, LOW);
    analogWrite(L_ENABLE_PIN, speed); //Make both motors run at spinning driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    delay(duration);
}

void left(int speed, int duration) {
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set both the left and right motors to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, HIGH);
    digitalWrite(R_REVERSE_PIN, LOW);
    analogWrite(L_ENABLE_PIN, speed); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    delay(duration);
}

void right(int speed, int duration) {
    digitalWrite(L_FORWARDS_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    digitalWrite(R_FORWARDS_PIN, LOW);
    digitalWrite(R_REVERSE_PIN, HIGH);
    analogWrite(L_ENABLE_PIN, speed); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    delay(duration);
}

void stop() {
    digitalWrite(L_FORWARDS_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, LOW);
    digitalWrite(R_REVERSE_PIN, LOW);
    analogWrite(L_ENABLE_PIN, 0); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, 0);
    }
    
/* ( End ) */
