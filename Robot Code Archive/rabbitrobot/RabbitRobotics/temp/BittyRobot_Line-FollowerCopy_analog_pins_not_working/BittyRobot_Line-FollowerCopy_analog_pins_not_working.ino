/******************************************

   This program is meant to make the robot drive on a black line drawn on a light surface. Electrical tape lines seem to work best.
   The robot will not follow the line perfectly, and will instead turn itself to be approximately in line with it if it finds itself off-course.

   Sam Damron, RabbitRobots.com 2017

 ******************************************/

/*-----( Define constants )-----*/
const int L_ENABLE_PIN = 7; //The digital pins that the motor controller is attatched to on the MEGA
const int L_FORWARDS_PIN = 6;
const int L_REVERSE_PIN = 5;
const int R_FORWARDS_PIN = 3;
const int R_REVERSE_PIN = 4;
const int R_ENABLE_PIN = 2;

//The digital pins the IR line sensors are attatched to on the MEGA

const int L_LINE_PIN = A3;
const int C_LINE_PIN = A2;
const int R_LINE_PIN = A1;

const int FAR_R_LINE_PIN = A0; //If your robot only has three IR line sensors, comment these two lines out an un-comment the two lines below:
const int FAR_L_LINE_PIN = A5;

//const int FAR_R_LINE_PIN = R_LINE_PIN;
//const int FAR_L_LINE_PIN = L_LINE_PIN;
int lineStandard = 650;

void setup() { /*----( SETUP: RUNS ONCE )----*/

  Serial.begin(9600);
  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_ENABLE_PIN, OUTPUT);
  pinMode(L_FORWARDS_PIN, OUTPUT);
  pinMode(L_REVERSE_PIN, OUTPUT);
  pinMode(R_FORWARDS_PIN, OUTPUT);
  pinMode(R_REVERSE_PIN, OUTPUT);
  pinMode(R_ENABLE_PIN, OUTPUT);

  //Set all of the pins connected to the IR line sensors to input mode so data can be read off of them
  pinMode(FAR_L_LINE_PIN, INPUT);
  pinMode(L_LINE_PIN, INPUT);
  pinMode(C_LINE_PIN, INPUT);
  pinMode(R_LINE_PIN, INPUT);
  pinMode(FAR_R_LINE_PIN, INPUT);

}

/*--(end setup )---*/



void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

Serial.println(analogRead(C_LINE_PIN));
/*Serial.print(analogRead(FAR_L_LINE_PIN));
Serial.print(digialRead(L_LINE_PIN));
Serial.print(digitalRead(C_LINE_PIN));
Serial.print(digitalRead(R_LINE_PIN));
Serial.println(digitalRead(FAR_R_LINE_PIN));
*/

 if (analogRead(C_LINE_PIN) > lineStandard) { 
right();
  } 

  if (analogRead(R_LINE_PIN) > lineStandard || analogRead(FAR_R_LINE_PIN) > lineStandard) {    
left();
  } 

  if (analogRead(L_LINE_PIN) > lineStandard || analogRead(FAR_L_LINE_PIN) > lineStandard) {    
right();
  }

if (analogRead(L_LINE_PIN) > lineStandard && analogRead(C_LINE_PIN) > lineStandard) {
  stop(); } else {
    forward(); 
  }
 
}

void right() {
  digitalWrite(L_FORWARDS_PIN, HIGH); //Set the left motor to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    
    digitalWrite(R_FORWARDS_PIN, LOW); //Set the right motor to go in reverse
    digitalWrite(R_REVERSE_PIN, HIGH);

    analogWrite(L_ENABLE_PIN, 60); //Spin right at 23% speed
    analogWrite(R_ENABLE_PIN, 60);
}

void left() {

   digitalWrite(L_FORWARDS_PIN, LOW); //Set the left motor to go in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    
    digitalWrite(R_FORWARDS_PIN, HIGH); //Set the right motor to go forwards
    digitalWrite(R_REVERSE_PIN, LOW);

    analogWrite(L_ENABLE_PIN, 60); //Spin left at 23% speed 
    analogWrite(R_ENABLE_PIN, 60);
}

void stop() {
  //Set both the left and right motors to go forwards
    digitalWrite(L_FORWARDS_PIN, LOW);
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, LOW);
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, 0); //Drive forwards at 23% speed
    analogWrite(R_ENABLE_PIN, 0);
}

void forward() {
  //Set both the left and right motors to go forwards
    digitalWrite(L_FORWARDS_PIN, HIGH);
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, HIGH);
    digitalWrite(R_REVERSE_PIN, LOW);
    
    analogWrite(L_ENABLE_PIN, 60); //Drive forwards at 23% speed
    analogWrite(R_ENABLE_PIN, 60);
}
/* --(end main loop )-- */


/* ( End ) */


