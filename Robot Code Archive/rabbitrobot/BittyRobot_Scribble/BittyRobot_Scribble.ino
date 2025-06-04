/******************************************
 * 
 * This program is meant to make the robot drive in a circle while waving a pen attatched to a servo.
 * The resulting pattern as the robot moves should look spirograph-like after a set amount of time passes
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 * Modified LeRoy Miller, 2017
 ******************************************/

/*-----( Define constants )-----*/
const int L_ENABLE_PIN = 7; //The digital pins that the motor controller is attatched to on the MEGA
const int L_FORWARDS_PIN = 6;
const int L_REVERSE_PIN = 5;
const int R_FORWARDS_PIN = 3;
const int R_REVERSE_PIN = 4;
const int R_ENABLE_PIN = 2;

const int SERVO_PIN = 10; //The digital pin the data line of the servo is attatched to on the MEGA

/*-----( Declare variables )-----*/
int servoPosition = 0; //The position for the servo to go to, from 0-180;
int servoPositionDelta = 5; //The change in position for the servo to go to each loop, from -180 to 180.

void setup(){ /*----( SETUP: RUNS ONCE )----*/

//Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
pinMode(L_ENABLE_PIN, OUTPUT);
pinMode(L_FORWARDS_PIN, OUTPUT);
pinMode(L_REVERSE_PIN, OUTPUT);
pinMode(R_FORWARDS_PIN, OUTPUT);
pinMode(R_REVERSE_PIN, OUTPUT);
pinMode(R_ENABLE_PIN, OUTPUT);

pinMode(SERVO_PIN, OUTPUT); //Set the pin connected to the servo to run in output mode

digitalWrite(L_FORWARDS_PIN, HIGH); //Set the left and right motors to go forwards
digitalWrite(L_REVERSE_PIN, LOW);
digitalWrite(R_FORWARDS_PIN, HIGH);
digitalWrite(R_REVERSE_PIN, LOW);

analogWrite(L_ENABLE_PIN, 60); //Make the left motor run at full speed and the right motor run at 23% speed
analogWrite(R_ENABLE_PIN, 100); // This should make the robot turn in a circle clockwise

}

/*--(end setup )---*/



void loop(){ /*----( LOOP: RUNS CONSTANTLY )----*/

  analogWrite(SERVO_PIN, servoPosition); //Set the servo to the position held in servoPosition

  servoPosition += servoPositionDelta; //Add the current change in servo position to servoPosition
  servoPosition = constrain(servoPosition, 0, 180); //Set servoPosition to the minimum and maximum allowable positions (0 - 180)

  //If servoPosition is at either its minimum or maximum value, invert servoPositionDelta (flip direction of movement)
  if(servoPosition == 0 || servoPosition == 180) servoPositionDelta = -servoPositionDelta; 
  
}
/* --(end main loop )-- */


/* ( End ) */
