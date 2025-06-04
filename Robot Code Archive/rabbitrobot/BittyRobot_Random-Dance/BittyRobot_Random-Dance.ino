/******************************************
 * 
 * This program makes the robot randomly "dance" by randomly completing different pre-set actions.
 * The driveSpeed can be manually changed in the program or by adding additional actions that change the driveSpeed instead of doing an action.
 * 
 * The included actions are driving forwards, driving backwards, turning left, turning right, spinning left, and spinning right.
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Define constants )-----*/
#define SPIN_LEFT 0 //Macros representing the different actions the robot can make 
#define SPIN_RIGHT 1
#define DRIVE_FORWARDS 2
#define DRIVE_REVERSE 3
#define SPEED_UP 4
#define SLOW_DOWN 5

const int L_ENABLE_PIN = 7; //The digital pins that the motor controller is attatched to one the MEGA
const int L_FORWARDS_PIN = 6;
const int L_REVERSE_PIN = 5;
const int R_FORWARDS_PIN = 4;
const int R_REVERSE_PIN = 3;
const int R_ENABLE_PIN = 2;

int maxSpeed = 175; //maximum speed
int stallSpeed = 65; //slowest speed

/*-----( Declare variables )-----*/

int driveSpeed = 100; //Motor speed for the robot; can range from 255 (max power) to 0 (stop).

void setup() { /*----( SETUP: RUNS ONCE )----*/

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_ENABLE_PIN, OUTPUT);
  pinMode(L_FORWARDS_PIN, OUTPUT);
  pinMode(L_REVERSE_PIN, OUTPUT);
  pinMode(R_FORWARDS_PIN, OUTPUT);
  pinMode(R_REVERSE_PIN, OUTPUT);
  pinMode(R_ENABLE_PIN, OUTPUT);

  randomSeed(analogRead(6)); //Generates a random seed based on electrical noise from the unused analog pin 6.
}

/*--(end setup )---*/



void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

  int action = random(0, 6); //Generate a random action for the robot to complete from 0-5 (SPIN_LEFT - DRIVE_REVERSE)
  
  int duration = random(500, 2001); //Generate a random duration of time in milliseconds for the action to run for from 500ms to 5000ms

  if (action == SPIN_LEFT) { //Code block for spinning left

    digitalWrite(L_FORWARDS_PIN, HIGH); //Set the left motor to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    
    digitalWrite(R_FORWARDS_PIN, LOW); //Set the right motor to go in reverse
    digitalWrite(R_REVERSE_PIN, HIGH);
    
    analogWrite(L_ENABLE_PIN, driveSpeed); //Make both motors run at spinning driveSpeed
    analogWrite(R_ENABLE_PIN, driveSpeed);
    
    delay(duration); //Don't do anything else until randomly-generated time has elapsed

  } 
  
  else if (action == SPIN_RIGHT) { //Code block for spinning right
    
    digitalWrite(L_FORWARDS_PIN, LOW); //Set the left motor to in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    
    digitalWrite(R_FORWARDS_PIN, HIGH); //Set the right motor to go forwards
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, driveSpeed); //Make both motors run at spinning driveSpeed
    analogWrite(R_ENABLE_PIN, driveSpeed);
    
    delay(duration);
  } 
  
  else if (action == SPEED_UP) { //Code block for speeding up
    
   driveSpeed += 50; //Increase driveSpeed by 50

   driveSpeed = constrain(driveSpeed, stallSpeed, maxSpeed); //Set driveSpeed into the allowable range of speeds if it goes over or under
    
  } 
  
  else if (action == SLOW_DOWN) { //Code block for slowing down
    
   driveSpeed -= 50; //Decrease driveSpeed by 50

   driveSpeed = constrain(driveSpeed, stallSpeed, maxSpeed); //Set driveSpeed into the allowable range of speeds if it goes over or under
  } 
  
  else if (action == DRIVE_FORWARDS) { //Code block for driving forwards
    
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set both the left and right motors to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    
    digitalWrite(R_FORWARDS_PIN, HIGH);
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, driveSpeed); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, driveSpeed);
    
    delay(duration);
  }
  
  else if (action == DRIVE_REVERSE) { //Code block for driving in reverse
    
    digitalWrite(L_FORWARDS_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    
    digitalWrite(R_FORWARDS_PIN, LOW);
    digitalWrite(R_REVERSE_PIN, HIGH);
    
    analogWrite(L_ENABLE_PIN, driveSpeed); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, driveSpeed);
    
    delay(duration);
  }
  
}
/* --(end main loop )-- */



/* ( End ) */
