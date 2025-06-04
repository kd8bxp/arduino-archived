/******************************************
 * 
 * This program makes the robot randomly "dance" by randomly completing different pre-set actions.
 * The speed can be manually changed in the program or by adding additional actions that change the speed instead of doing an action.
 * 
 * The included actions are driving forwards, driving backwards, turning left, turning right, spinning left, and spinning right.
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Define constants )-----*/
#define SPIN_LEFT 0 //Macros representing the different actions the robot can make 
#define SPIN_RIGHT 1
#define TURN_LEFT 2
#define TURN_RIGHT 3
#define DRIVE_FORWARDS 4
#define DRIVE_REVERSE 5

const int L_ENABLE_PIN = 11; //The digital pins that the motor controller is attatched to one the UNO
const int L_FORWARDS_PIN = 10;
const int L_REVERSE_PIN = 9;
const int R_FORWARDS_PIN = 8;
const int R_REVERSE_PIN = 7;
const int R_ENABLE_PIN = 6;


/*-----( Declare variables )-----*/

/*
   Motor speeds for each action the robot can do.
   Can range from 255 (max power) to 0 (stop).
   These aren't changed in the program as it is, but the code can be modified to allow them to be changed later on

   turnSpeedFast refers to the speed of the leading motor (e.g. the left motor for left turns)
   while turnSpeedSlow refers to the speeed of the trailing motor
*/
byte driveSpeed = 255;
byte spinSpeed = 255;
byte turnSpeedFast = 255;
byte turnSpeedSlow = 127;

void setup() { /*----( SETUP: RUNS ONCE )----*/

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_ENABLE_PIN, OUTPUT);
  pinMode(L_FORWARDS_PIN, OUTPUT);
  pinMode(L_REVERSE_PIN, OUTPUT);
  pinMode(R_FORWARDS_PIN, OUTPUT);
  pinMode(R_REVERSE_PIN, OUTPUT);
  pinMode(R_ENABLE_PIN, OUTPUT);

  randomSeed(analogRead(5)); //Generates a random seed based on electrical noise from the unused analog pin 5.
}

/*--(end setup )---*/



void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

  int action = random(0, 6); //Generate a random action for the robot to complete from 0-5 (SPIN_LEFT - DRIVE_REVERSE)
  
  int duration = random(500, 5001); //Generate a random duration of time in milliseconds for the action to run for from 500ms to 5000ms

  if (action == SPIN_LEFT) { //Code block for spinning left

    digitalWrite(L_FORWARDS_PIN, HIGH); //Set the left motor to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    
    digitalWrite(R_FORWARDS_PIN, LOW); //Set the right motor to go in reverse
    digitalWrite(R_REVERSE_PIN, HIGH);
    
    analogWrite(L_ENABLE_PIN, spinSpeed); //Make both motors run at spinning speed
    analogWrite(R_ENABLE_PIN, spinSpeed);
    
    delay(duration); //Don't do anything else until randomly-generated time has elapsed

  } 
  
  else if (action == SPIN_RIGHT) { //Code block for spinning right
    
    digitalWrite(L_FORWARDS_PIN, LOW); //Set the left motor to in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    
    digitalWrite(R_FORWARDS_PIN, HIGH); //Set the right motor to go forwards
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, spinSpeed); //Make both motors run at spinning speed
    analogWrite(R_ENABLE_PIN, spinSpeed);
    
    delay(duration);
  } 
  
  else if (action == TURN_LEFT) { //Code block for turning left
    
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set both the left and right motors to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    
    digitalWrite(R_FORWARDS_PIN, HIGH);
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, turnSpeedSlow); //Have the left motor run faster than the right, making the robot turn
    analogWrite(R_ENABLE_PIN, turnSpeedFast);
    
    delay(duration);
  } 
  
  else if (action == TURN_RIGHT) { //Code block for turning right
    
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set both the left and right motors to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    
    digitalWrite(R_FORWARDS_PIN, HIGH);
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, turnSpeedFast); //Have the right motor run faster than the left, making the robot turn
    analogWrite(R_ENABLE_PIN, turnSpeedSlow);
    
    delay(duration);
  } 
  
  else if (action == DRIVE_FORWARDS) { //Code block for driving forwards
    
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set both the left and right motors to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    
    digitalWrite(R_FORWARDS_PIN, HIGH);
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, driveSpeed); //Set both the left and right motors to run at driving speed
    analogWrite(R_ENABLE_PIN, driveSpeed);
    
    delay(duration);
  }
  
  else if (action == DRIVE_REVERSE) { //Code block for driving in reverse
    
    digitalWrite(L_FORWARDS_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    
    digitalWrite(R_FORWARDS_PIN, LOW);
    digitalWrite(R_REVERSE_PIN, HIGH);
    
    analogWrite(L_ENABLE_PIN, driveSpeed); //Set both the left and right motors to run at driving speed
    analogWrite(R_ENABLE_PIN, driveSpeed);
    
    delay(duration);
  }
  
}
/* --(end main loop )-- */



/* ( End ) */
