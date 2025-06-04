/******************************************
 * 
 * This program is meant to demonstrate how to use stepper motors using an Arduino UNO with ULN2003 stepper motor driver and 
 * 28BYJ-48 stepper motor over serial. A different stepper motor can be used with different steps per revolution and max/min RPM
 * 
 * A computer with the Ardunio IDE installed and a USB-D to USB-A cable is needed to be able to use this program. 
 * 
 * The RPM and steps to move can be changed over serial with the given commands
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

 /*-----( Import required libraries )-----*/
#include <Stepper.h> //Library for using stepper motors

/*-----( Define constants )-----*/
#define CMD_NONE 0 //Constants reresenting the different commands that can be enteed via serial
#define CMD_RPM 1
#define CMD_STEPS 2
#define CMD_RUN 3
#define CMD_INFO 4

/*-----( Declare objects )-----*/

/*
 * 
 * Setup the stepper motor we will be using (stepsPerRevolution, d0, d1, d2, d3)
 * The digital pins should connect to the provided stepper motor driver and not the stepper motor itself.
 * If you use a stepper motor other than the ULN2003, check to find the correct steps per revollution according to your manufacturer
 * 
 * Caution: Use an external 5V power supply for the stepper motor to avoid potentially drawing too
 * much current from the Arduino UNO and causing damage. Stepper motor becomes hot during use; do not handle while running.
 * 
 */
Stepper stepperMotor = Stepper(2048, 2, 3, 4, 5);

/*-----( Declare variables )-----*/
int stepperRPM = 20; //Stores the stepper motor's RPM. The ULN2003 stepper motor can function at any speed between 1-20 RPM
long steps = 0; //Stores the steps to be ran by the motor
int command = 0; //Stores the command entered into serial using the constants defined above
String serialString = ""; //Stores the literal command entered into serial as a string

void setup() { /*----( SETUP: RUNS ONCE )----*/

  Serial.begin(9600);//Begin serial connection at 9600 baud
  
  Serial.println("Guide:"); //Print out a brief user's guide to the different enterable commands.
  Serial.println("setRPM <speed>: Sets the RPM of the stepper motor. Must be a positive number from 1-20");
  Serial.println("setSteps <number>: Sets the number of steps the stepper motor is to use. Must be a positive number");
  Serial.println("doSteps: Makes the stepper motor complete the number of steps entered at its set speed");
  Serial.println("showInfo: Displays the set RPM and step count");

  pinMode(2, OUTPUT); //Prepare the four digital pins to be used for the stepper motor
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

}
/*--(end setup )---*/

void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/
  
 if(Serial.available() > 0){ //If there are characters in the serial buffer, do the following:

  /*----( Begin serial dependant section )----*/

  if(command == CMD_NONE){ //If there is no current command running

    serialString = Serial.readStringUntil(' ');//Get the first "word" from the serial buffer and store it as a string

    if(serialString == "setRPM") command = CMD_RPM; //If the user user typed "setRPM" state that the user is using that command

    else if(serialString == "setSteps") command = CMD_STEPS; //If the user user typed "setSteps" state that the user is using that command

    else if(serialString == "doSteps") command = CMD_RUN; //If the user user typed doSteps" state that the user is using that command
    
    else if(serialString == "showInfo") command = CMD_INFO; //If the user user typed "showInfo" state that the user is using that command

    
  }
  
  if(command == CMD_RPM){//If the RPM is supposed to be set, do the following:
    
    serialString = Serial.readString(); //Get the remainer of the serial data entered as a string
    serialString.trim(); //Remove extra whitespace on the left and right of the string
    stepperRPM = serialString.toInt(); //Parse the string as an integer if possible
    if(stepperRPM < 1){ //If the user tries to set an RPM less than one, tell them that cannot be done and set the RPM to one instead
      Serial.println("Error: Stepper RPM must be an integer above 0. Setting RPM to 1.");
      stepperRPM = 1;
    }
    Serial.println("RPM set"); //State that the RPM has been set for the stepper motor
 
  }
  
  else if(command == CMD_STEPS){ //If the number of steps is suppsoed to be set, do the following:

    serialString = Serial.readString(); //Get the remainer of the serial data entered as a string
    serialString.trim(); //Remove extra whitespace on the left and right of the string
    steps = serialString.toInt(); //Parse the string as an integer if possible
    Serial.println("Steps Set"); //State that the queued steps have been set for the stepper motor
    
  }
 
 } /*----( End serial data dependant section )----*/



 else{ //Otherwise, if there is not data in the serial buffer, state that there is not command currently running
   command = CMD_NONE;
 }

 if(command == CMD_RUN){ //If the current command is doSteps, do the following:
  stepperMotor.step(steps); //Have the stepper motor turn the number of steps specified
  Serial.println("Steps completed"); //State that the motor has moved the required steps
 }
 else if(command == CMD_INFO){ //If the current command s showInfo, do the following:
  
  Serial.print("RPM: "); //Print out the set RPM and steps for the motor
  Serial.print(stepperRPM);
  Serial.print("   ");
  Serial.print("Steps: ");
  Serial.println(steps);
  
 }
 
 stepperMotor.setSpeed(stepperRPM); //Update the desired speed for the motor at the end of the main loop

}
/* --(end main loop )-- */



/* ( End ) */
