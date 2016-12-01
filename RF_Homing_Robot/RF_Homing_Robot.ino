//July 8, 2015 - Based on code written by Warner K. of www.robowarner.com
//July 8, 2015 - by LeRoy Miller
//Changes: Using motors, not servos, no longer using boe-bot, using a 433mhz reciever, and transmitter.

//Code written by Warner K. of www.robowarner.com
//Made for the Radio Direction Finding Robot at www.robowarner.com/robot/rdfbot
//You are free to modify, distribute and store this code. 
//
//USE THIS CODE AT YOUR OWN RISK!
//I am NOT liable for any harm that results from the use of this code. 
//
// Code is based off of the original BASIC for the robot, so it has room for improvement. :)

//Defines (Constants)
#define buffering 25 //what counts as straight ahead? If too small, the robot will jitter. If too large the robot will drive away from the transmitter
//pinout
#define ANT_WAVEFORM_OUT 8 // set output pin
#define SPEAKER_FROM_WALKIETALKIE A2 //set input pin
#define CALIBRATE_IN 11
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

//Variables
uint16_t caliset = 0;
uint16_t voltage = 0;
int slow = 64;
int medium = 128;
int fast = 255;


//start program
void setup() {
analogReference(DEFAULT);
 
Serial.begin(9600); //begin serial monitor output, remove if you don't want it
delay(6000); // I put this delay here so the robot won't move until the walkie talkie has had time to turn on.
pinMode(ANT_WAVEFORM_OUT, OUTPUT);
pinMode(SPEAKER_FROM_WALKIETALKIE, INPUT);
pinMode(CALIBRATE_IN, INPUT);
digitalWrite(CALIBRATE_IN, HIGH); // enable internal pullup resistor
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);

//read calibration word from EEPROM
byte HByte =  0;
byte LByte =  162;
caliset = word(HByte, LByte);
//caliset = 917;
Serial.print("EEPROM Calibration number: ");
Serial.print(caliset);
Serial.println(" If you haven't calibrated yet, you need to for it to work");

if (digitalRead(CALIBRATE_IN) == LOW){ // used for calibrating
delay(9000); // a wait so you can back away from the robot while it is calibrating
}
}

void loop(){
digitalWrite(ANT_WAVEFORM_OUT, HIGH);  //output antenna switching waveform
delay(1);
digitalWrite(ANT_WAVEFORM_OUT, LOW);
delay(1);
digitalWrite(ANT_WAVEFORM_OUT, HIGH);
delay(1);
digitalWrite(ANT_WAVEFORM_OUT, LOW);
delay(1);
digitalWrite(ANT_WAVEFORM_OUT, HIGH);
delay(1);
digitalWrite(ANT_WAVEFORM_OUT, LOW);
delay(1);
digitalWrite(ANT_WAVEFORM_OUT, HIGH);
delay(1);
digitalWrite(ANT_WAVEFORM_OUT, LOW);
delay(1);
voltage = analogRead(SPEAKER_FROM_WALKIETALKIE); //read voltage from radio
if (digitalRead(CALIBRATE_IN) == LOW){           //if in calibrate mode, store voltage in EEPROM
caliset = voltage;
byte HByte = highByte(caliset);
byte LByte = lowByte(caliset);
Serial.print ("highbyte: ");
Serial.println(HByte);
Serial.print("lowByte: ");
Serial.println(LByte);
Serial.print("Caliset: ");
Serial.println(caliset);
delay(5000);
endprogram();
}

if (voltage > (caliset - buffering) && voltage < (caliset + buffering)) { //drive forward

//motor control code for FORWARD here
//code for a continuous roating servo is included below
analogWrite(IN1, 0);
 analogWrite(IN2, fast);
 
 analogWrite(IN3, fast-30);
 analogWrite(IN4, 0);
Serial.println("FORWARD:");
}

if (voltage > (caliset + buffering)){ //turn

//motor control code for TURNING here (right or left depends on antenna config.)
//code for a continuous roating servo is included below
analogWrite(IN1, 0); //left
analogWrite(IN2, fast);//left
analogWrite(IN3, 0);//left
analogWrite(IN4, fast);//left
Serial.println("LEFT:");

}
if (voltage < (caliset - buffering)){  //turn the other way

//motor control code for TURNING the OTHER DIRECTION here (right or left depends on antenna config.)
//code for a continuous roating servo is included below
analogWrite(IN1, fast); //right
analogWrite(IN2, 0); //right
analogWrite(IN3, fast); //right
analogWrite(IN4, 0); //right
Serial.println("RIGHT");

}
delay(5); //just a simple wait
Serial.print("In, Cal\t");
Serial.print(voltage);
Serial.print("\t");
Serial.println(caliset);
}

void endprogram(){
  Serial.print("Calibrated to: ");
Serial.print(caliset);
  loopy:
  goto loopy;
}
