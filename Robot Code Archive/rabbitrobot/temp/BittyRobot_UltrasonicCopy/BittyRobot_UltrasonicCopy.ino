/******************************************
 * 
 * This program is meant to make the robot drive around while avoiding obstacles using its ultrasonic sensor.
 * If an object is deteced to be witihin an inch of it, the robot will tunr left until there is nothing in the way.
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Define constants )-----*/
const int L_ENABLE_PIN = 7; //The digital pins that the motor controller is attatched to on the MEGA
const int L_FORWARDS_PIN = 6;
const int L_REVERSE_PIN = 5;
const int R_FORWARDS_PIN = 3;
const int R_REVERSE_PIN = 4;
const int R_ENABLE_PIN = 2;

const int US_TRIGGER_PIN = 23; //The digital pins that the ultrasonic sensor is connected to on the MEGA
const int US_RECEIVE_PIN = 25;

const int DURATION_PER_INCH = 550; //The approximate duration in microseconds of the echo for an object one inch away.


/*-----( Declare variables )-----*/
unsigned long echoDuration = 0; //Used to store the duration of the echo in microseconds

void setup(){ /*----( SETUP: RUNS ONCE )----*/

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_ENABLE_PIN, OUTPUT);
  pinMode(L_FORWARDS_PIN, OUTPUT);
  pinMode(L_REVERSE_PIN, OUTPUT);
  pinMode(R_ENABLE_PIN, OUTPUT);
  pinMode(R_FORWARDS_PIN, OUTPUT);
  pinMode(R_REVERSE_PIN, OUTPUT);

  //Set the pin connected to the trigger of the ultrasound sensor to be output to send the pulse and set the receiver to input to read the echo coming back
  pinMode(US_TRIGGER_PIN, OUTPUT);
  pinMode(US_RECEIVE_PIN, INPUT);
}

/*--(end setup )---*/



void loop(){ /*----( LOOP: RUNS CONSTANTLY )----*/

  digitalWrite(US_TRIGGER_PIN, HIGH); //Send voltage to the trigger pin to begin the ultrasound pusle

  delayMicroseconds(10); //Pause the program for 10 microseconds (to produce a 10 microsecond pulse)

  digitalWrite(US_TRIGGER_PIN, LOW); //Stop sendig voltage to the trigger pin, ending the pulse

  echoDuration = pulseIn(US_RECEIVE_PIN, HIGH); //Get the duration of the echo coming back in microseconds 

  if(echoDuration > DURATION_PER_INCH){ //If the object is over an inch away:

    digitalWrite(L_FORWARDS_PIN, HIGH); //Set the left motor to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);

    analogWrite(L_ENABLE_PIN, 155); //Run the left motor at 60%

    digitalWrite(R_FORWARDS_PIN, HIGH); //Set the right motor to go forwards
    digitalWrite(R_REVERSE_PIN, LOW);

    analogWrite(R_ENABLE_PIN, 155); //Run the right motor at 60% speed    
  } 
  
  else if(echoDuration <= DURATION_PER_INCH){ //Otherwise, if the object is less than na inch away:
    
    digitalWrite(L_FORWARDS_PIN, LOW); //Set the left motor to go in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    
    analogWrite(L_ENABLE_PIN, 155); //Run the left motor at 60% speed

    digitalWrite(R_FORWARDS_PIN, HIGH); //Set the right motor go in reverse
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(R_ENABLE_PIN, 155); //Run the right motor at 60% speed

    delay(500); //Keep turning for 1/2 of a second
    
  }
  
}
/* --(end main loop )-- */


/* ( End ) */
