// Bluetooth-bot v1
// Arduino Robotics unofficial chapter 14
// use Bluetooth Mate serial adapter to receive commands from PC
// Arduino decodes commands into motor movements
// Creates high-speed wireless serial link for robot control using keyboard
// Uses keys "i" = forward, "j" = left, "k" = reverse, and "l" = right
// speed control is also implemented using "," = speed down, "." = speed up, and "/" = max speed.


// L298 motor control variables
int M1_A = 12;
int M1_PWM = 11;
int M1_B = 10;

int M2_A = 4;
int M2_PWM = 3;
int M2_B = 2;


// LED pin attached to Arduino D13
int LED = 13;

// variable to store serial data
int incomingByte = 0;

// variable to store speed value
int speed_val = 255;

//////////////////////////////


void setup(){

TCCR2B = TCCR2B & 0b11111000 | 0x01; // change PWM frequency for pins 3 and 11 to 32kHz so there will be no motor whining

// Start serial monitor at 115,200 bps
Serial.begin(115200);

// declare outputs
pinMode(LED, OUTPUT);

pinMode(M1_A, OUTPUT);
pinMode(M1_PWM, OUTPUT);
pinMode(M1_B, OUTPUT);

pinMode(M2_A, OUTPUT);
pinMode(M2_PWM, OUTPUT);
pinMode(M2_B, OUTPUT);

// turn motors Off by default
M1_stop();
M2_stop();

delay(500);

}

////////////////////////////////////

void loop(){

// check for serial data
if (Serial.available() > 0) {
// read the incoming byte:
incomingByte = Serial.read();
// if available, blink LED and print serial data received.
digitalWrite(LED, HIGH);
// say what you got:
Serial.print("I received: ");
Serial.println(incomingByte);
// delay 10 milliseconds to allow serial update time
delay(10);

// check incoming byte for direction
// if byte is equal to "46" or "," - raise speed
if (incomingByte == 46){
speed_val = speed_val + 5;
test_speed();
Serial.println(speed_val);
}
// if byte is equal to "44" or "." - lower speed
else if (incomingByte == 44){
speed_val = speed_val - 5;
test_speed();
Serial.println(speed_val);
}
// if byte is equal to "47" or "/" - max speed
else if (incomingByte == 47){
speed_val = 255;
test_speed();
}

// if byte is equal to "105" or "i", go forward
else if (incomingByte == 105){
M1_forward(speed_val);
M2_forward(speed_val);
delay(25);
}
// if byte is equal to "106" or "j", go left
else if (incomingByte == 106){
M1_reverse(speed_val);
M2_forward(speed_val);
delay(25);
}
// if byte is equal to "108" or "l", go right
else if (incomingByte == 108){
M1_forward(speed_val);
M2_reverse(speed_val);
delay(25);
}
// if byte is equal to "107" or "k", go reverse
else if (incomingByte == 107){
M1_reverse(speed_val);
M2_reverse(speed_val);
delay(25);
}
// otherwise, stop both motors
else {
M1_stop();
M2_stop();
}


}

else {
M1_stop();
M2_stop();
digitalWrite(LED, LOW);
}
}

void test_speed(){
// constrain speed value to between 0-255
if (speed_val > 250){
speed_val = 255;
Serial.println(" MAX ");
}
if (speed_val < 0){
speed_val = 0;
Serial.println(" MIN ");
}

}

/////////// motor functions ////////////////

void M1_reverse(int x){
digitalWrite(M1_B, LOW);
digitalWrite(M1_A, HIGH);
analogWrite(M1_PWM, x);
}

void M1_forward(int x){
digitalWrite(M1_A, LOW);
digitalWrite(M1_B, HIGH);
analogWrite(M1_PWM, x);
}

void M1_stop(){
digitalWrite(M1_B, LOW);
digitalWrite(M1_A, LOW);
digitalWrite(M1_PWM, LOW);
}

void M2_forward(int y){
digitalWrite(M2_B, LOW);
digitalWrite(M2_A, HIGH);
analogWrite(M2_PWM, y);
}

void M2_reverse(int y){
digitalWrite(M2_A, LOW);
digitalWrite(M2_B, HIGH);
analogWrite(M2_PWM, y);
}

void M2_stop(){
digitalWrite(M2_B, LOW);
digitalWrite(M2_A, LOW);
digitalWrite(M2_PWM, LOW);
}







