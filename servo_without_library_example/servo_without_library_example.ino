int servopin=3;// define the digital 9 is connected to servo signal line 
int myangle;// define the angle variable
int pulsewidth;// define the pulsewidth variable
int val;
void setup()
{
pinMode(servopin,OUTPUT);// set the servo interface as OUTPUT
Serial.begin(9600);// connect to serial port, baud rate to 9600
Serial.println("servo=o_seral_simple ready" );
servopulse(servopin,90);// call the pulse function, make the servo rotate to 90degree
} 
void loop()
{
  servopulse(servopin,90);// call the pulse function, make the servo rotate to 90degree
}
void servopulse(int servopin,int myangle)// define a pulse function 
{
pulsewidth=(myangle*11)+500;// convert the angle into pulse width of 500-2480 
digitalWrite(servopin,HIGH);// set the servo pin to HIGH 
delayMicroseconds(pulsewidth);// delay the microseconds of pulsewidth
digitalWrite(servopin,LOW);// set the servo pin to LOW
delay(20-pulsewidth/1000);
}
