// ArchReactor Pixybot
// Written by: Apollo W. Timbers
// 2015
//
// Code revision mark VI
//
// This code is a intigration of the Pixy vision system into a mobile platform to raise awerness of the maker space
// know as Arch Reactor.
// Sections of the code include following tracked objects, IR sensor object detecting, A PID control loop,
// A low battery warning/cuttoff. and pausing the drive servos so you can train the Pixy.
//
// Portions of this code are derived from Adafruit Industries Pixy Pet code.
// Portions of this code are derived from the Pixy CMUcam5 pantilt example code.
//
 
#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>
 
#define X_CENTER 160L
#define Y_CENTER 100L
#define RCS_MIN_POS 0L
#define RCS_MAX_POS 1000L
#define RCS_CENTER_POS ((RCS_MAX_POS-RCS_MIN_POS)/2)
 
// number of analog samples to take per reading
#define NUM_SAMPLES 10
 
// constants won't change. They're used here to
// set pin numbers:
int sum = 0; // sum of samples taken
unsigned char sample_count = 0; // current sample number
const int buttonPin = 2; // the number of the pushbutton pin
const int ledPin = 111; // the pin number of the LED pin
int ledgreen = 5; // the pin number of the LED pin
int ledred = 7; // the pin number of the LED pin
// variables will change:
int buttonState = 0; // variable for reading the pushbutton status
Servo leftServo; // Define the Servos
Servo rightServo;
//Servo gripperServo;
int analogInput = A0;
unsigned int raw;
double vcc = 0;
double voltage; // calculated voltage
const int irSenseleft = A1; // Connect sensor to analog pin A0
int distanceleft = 0;
const int irSenseright = A2; // Connect sensor to analog pin A0
int distanceright = 0;
class ServoLoop
 
{
public:
 ServoLoop(int32_t pgain, int32_t dgain);
 
 void update(int32_t error);
 
 int32_t m_pos;
 int32_t m_prevError;
 int32_t m_pgain;
 int32_t m_dgain;
};
 
ServoLoop panLoop(200, 200);
ServoLoop tiltLoop(150, 200);
 
ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
{
 m_pos = RCS_CENTER_POS;
 m_pgain = pgain;
 m_dgain = dgain;
 m_prevError = 0x80000000L;
}
 
void ServoLoop::update(int32_t error)
{
 long int vel;
 char buf[32];
 if (m_prevError!=0x80000000)
 {
 vel = (error*m_pgain + (error - m_prevError)*m_dgain)>>10;
 //sprintf(buf, "%ld\n", vel);
 //Serial.print(buf);
 m_pos += vel;
 if (m_pos>RCS_MAX_POS)
 m_pos = RCS_MAX_POS;
 else if (m_pos<RCS_MIN_POS)
 m_pos = RCS_MIN_POS;
 //cprintf("%d %d %d\n", m_axis, m_pos, vel);
 }
 m_prevError = error;
}
 
Pixy pixy;
 
void setup()
{
 Serial.begin(9600);
 Serial.print("Starting...\n");
 leftServo.attach(13); // servo on digital pin 9
 rightServo.attach(10); // servo on digital pin 10
 // gripperServo.attach(11); // servo on digital pin 11
 pixy.init();
 pinMode(ledPin, OUTPUT);
 // initialize the pushbutton pin as an input:
 pinMode(buttonPin, INPUT);
 pinMode(ledgreen, OUTPUT);
 pinMode(ledred, OUTPUT);
 digitalWrite(ledred, LOW);
 digitalWrite(ledgreen, LOW);
 //analogReference(INTERNAL);
 delay (500);
 Serial.print("Checking internal voltage...\n");
 readVcc(); // Check initial voltage
 Voltage();
}
 
uint32_t hold = 0;
int32_t size = 400;
uint32_t lastBlockTime = 0;
//---------------------------------------
// Main loop - runs continuously after setup
//---------------------------------------
void loop()
{
 static unsigned long timer=millis(); // initialize timer variable and set to millisecond timer
 static unsigned long timerA=millis(); // initialize timer variable and set to millisecond timer
 static int i = 0;
 int j;
 uint16_t blocks;
 char buf[32];
 int32_t panError, tiltError;
 blocks = pixy.getBlocks();
 // If we have blocks in sight, track and follow them
 if (blocks)
 {
 int trackedBlock = TrackBlock(blocks);
 FollowBlock(trackedBlock);
 lastBlockTime = millis();
 size += pixy.blocks[trackedBlock].width * pixy.blocks[trackedBlock].height;
 size -= size >> 3;
 if (size >= 500);
 // Grip();
 // if (millis() - hold > random(3000, 7000))
 // Drop();
 }
 else if (millis() - lastBlockTime > 3000)
 {
 leftServo.writeMicroseconds(1510);
 rightServo.writeMicroseconds(1510);
 ScanForBlocks();
 }
 if(millis()-timer>2999) // if at least 3000mS have passed
 {
 timer=millis(); // reset timer
 readVcc();
 Voltage();
 }
 if(millis()-timerA>49) //Wait 50 ms between each read for IR
 // According to datasheet time between each read
 // is -38ms +/- 10ms. Waiting 50 ms assures each
 // read is from a different sample
 {
 timerA=millis(); // reset timer
 Pausebutton();
 irRead();
 }
 if (voltage >= 6){
 digitalWrite(ledgreen, HIGH);
 }
 else if ((voltage >= 5) && (voltage < 6))
 {
 digitalWrite(ledgreen, LOW);
 digitalWrite(ledred, HIGH);
 }
 else if (voltage < 5)
 {
 leftServo.writeMicroseconds(1510); // Hold motors because of low poer state
 rightServo.writeMicroseconds(1510);
 digitalWrite(ledgreen, LOW);
 digitalWrite(ledred, HIGH); // LED to warn of low battery
 delay (500);
 digitalWrite(ledred, LOW);
 delay (500);
 }
 
}
 
int oldX, oldY, oldSignature;
 
//---------------------------------------
// Track blocks via the Pixy pan/tilt mech
// (based in part on Pixy CMUcam5 pantilt example)
//---------------------------------------
int TrackBlock(int blockCount)
{
 int trackedBlock = 0;
 long maxSize = 0;
 uint32_t hold = 0;
 
 Serial.print("blocks =");
 Serial.println(blockCount);
 
 for (int i = 0; i < blockCount; i++)
 {
 if ((oldSignature == 0) || (pixy.blocks[i].signature == oldSignature))
 {
 long newSize = pixy.blocks[i].height * pixy.blocks[i].width;
 if (newSize > maxSize)
 {
 trackedBlock = i;
 maxSize = newSize;
 }
 }
 }
 
 int32_t panError = X_CENTER - pixy.blocks[trackedBlock].x;
 int32_t tiltError = pixy.blocks[trackedBlock].y - Y_CENTER;
 
 panLoop.update(panError);
 tiltLoop.update(tiltError);
 
 pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
 
 oldX = pixy.blocks[trackedBlock].x;
 oldY = pixy.blocks[trackedBlock].y;
 oldSignature = pixy.blocks[trackedBlock].signature;
 return trackedBlock;
}
 
void FollowBlock(int trackedBlock)
{
 int32_t followError = pixy.blocks[0].x-X_CENTER; // How far off-center are we looking now?
 
 // Size is the area of the object.
 // We keep a running average of the last 8.
 size += pixy.blocks[trackedBlock].width * pixy.blocks[trackedBlock].height;
 size -= size >> 3;
 
 // Forward speed decreases as we approach the object (size is larger)
 int forwardSpeed = constrain(400 - (size/400), -100, 400); 
 
 // Steering differential is proportional to the error times the forward speed
 int32_t differential = (followError + (followError * forwardSpeed))>>4;
 
 // Adjust the left and right speeds by the steering differential.
 int leftSpeed = constrain(forwardSpeed + differential, -400, 400);
 int rightSpeed = constrain(forwardSpeed - differential, -400, 400);
 
leftSpeed = map(leftSpeed,-400,400,1650,1350); // Map to servo output values
rightSpeed = map(rightSpeed,-400,400,1350,1650); // Map to servo output values 
 
 // Update servos
 leftServo.writeMicroseconds(leftSpeed);
 rightServo.writeMicroseconds(rightSpeed);
}
 
int scanIncrement = (RCS_MAX_POS - RCS_MIN_POS) / 150;
uint32_t lastMove = 0;
//---------------------------------------
// Random search for blocks
//
// This code rotates slowly in a circle
// until a block is detected
//---------------------------------------
void ScanForBlocks()
{
 if (millis() - lastMove > 10)
 {
 lastMove = millis();
 //panLoop.m_pos += scanIncrement;
 //if ((panLoop.m_pos >= RCS_MAX_POS)||(panLoop.m_pos <= RCS_MIN_POS))
 //{
 tiltLoop.m_pos =(RCS_MAX_POS * 0.6);
 //scanIncrement = -scanIncrement;
 //if (scanIncrement < 0)
 
 leftServo.writeMicroseconds(1460);
 rightServo.writeMicroseconds(1460);
 delay (20);
 
 pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
 }
} 
 
// Take multiple readings, and average them out to reduce false readings
void irRead()
{
	/*
 int averagingleft = 0; // Holds value to average readings
 int averagingright = 0; // Holds value to average readings
 
 // Get a sampling of 5 readings from sensor
 //for (int i=0; i<3; i++)
 {
 distanceleft = analogRead(irSenseleft);
 averagingleft = averagingleft + distanceleft;
 distanceright = analogRead(irSenseright);
 averagingright = averagingright + distanceright;
 }
 distanceleft = averagingleft / 5; // Average out readings
 //return(distanceleft); // Return value
 Serial.println(distanceleft, DEC);
 distanceright = averagingright / 5; // Average out readings
 //return(distanceright); // Return value
 Serial.println(distanceright, DEC);
 if (distanceleft > 85)
 {
 leftServo.writeMicroseconds(1510); //allstop
 rightServo.writeMicroseconds(1510);
 delay(200);
 leftServo.writeMicroseconds(1660); //backup
 rightServo.writeMicroseconds(1460);
 delay(500);
 leftServo.writeMicroseconds(1460); //spin
 rightServo.writeMicroseconds(1460);
 delay(300);
 
 }
 else if (distanceright > 85)
 {
 //allstop
 leftServo.writeMicroseconds(1510);
 rightServo.writeMicroseconds(1510);
 delay (200);
 }
 */
}
 
void Voltage()
{
 vcc = readVcc()/1000.0;
 raw = analogRead(analogInput);
 voltage = ((raw / 1023.0) * vcc) * 2;
 Serial.print ("Battery Voltage = ");
 Serial.println(voltage, DEC);
 Serial.print ("VCC Voltage = ");
 Serial.println(vcc, DEC );
}
 
long readVcc() {
 long result;
 // Read 1.1V reference against AVcc
 ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
 delay(2); // Wait for Vref to settle
 ADCSRA |= _BV(ADSC); // Convert
 while (bit_is_set(ADCSRA,ADSC));
 result = ADCL;
 result |= ADCH<<8;
 result = 1106400L / result; // Back-calculate AVcc in mV
 return result;
}
 
void Pausebutton()
{
	/*
 sample_count = 0;
 sum = 0;
 buttonState = digitalRead(buttonPin);
 
 // check if the pushbutton is pressed.
 // if it is, the buttonState is HIGH:
 if (buttonState == HIGH) {
 // turn LED on:
 leftServo.writeMicroseconds(1510);
 rightServo.writeMicroseconds(1510);
 delay(18000);
 }
 */
}
 
//byte haveball = 0;
//void Grip()
//{
// hold = millis();
 // leftServo.writeMicroseconds(1505);
 // rightServo.writeMicroseconds(1500);
 // delay(50);
 // leftServo.writeMicroseconds(1660);
 // rightServo.writeMicroseconds(1460);
 //delay(200);
 // leftServo.writeMicroseconds(1660);
 // delay(50);
 // haveball = 1;
//}
 
//void Drop()
//{
 
// leftServo.writeMicroseconds(1405);
// rightServo.writeMicroseconds(1600);
// delay(200);
// leftServo.writeMicroseconds(1460);
// rightServo.writeMicroseconds(1460);
// delay(200);
// leftServo.writeMicroseconds(160);
// delay(50);
// haveball = 0;
//}