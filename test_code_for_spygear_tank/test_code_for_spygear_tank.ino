//int ENA=13;//connected to Arduino's port 2
int IN1=8;//connected to Arduino's port 3
int IN2=9;//connected to Arduino's port 4
//int ENB=12;//connected to Arduino's port 5
int IN3=12;//connected to Arduino's port 6
int IN4=13;//connected to Arduino's port 7
int slow=64;
int medium=128;
int fast=255;
 
void setup()
{
 int myEraser = 7;
 TCCR2B &= ~myEraser;
 int myPrescaler = 2;
 TCCR2B |= myPrescaler;

 //pinMode(ENA,OUTPUT);
 //pinMode(ENB,OUTPUT);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT);
 pinMode(IN3,OUTPUT);
 pinMode(IN4,OUTPUT);
 //digitalWrite(ENA,HIGH);//enablae motorA
 //digitalWrite(ENB,HIGH);//enable motorB
}
void loop()
{/*In the way of 4 beats to drive the stepping motor,A group connected to motorA,B
 B group connected to motorB,Suppose A representing the forward current of A group,
 A- representing the reverse current of A group,B representing the forward current of B group,
 B- representing the reverse current of B group.
 this way run as follow:
 AB    A-B    A-B-   AB-
 or
 AB   AB-    A-B-   A-B
 */
 
 analogWrite(IN3,slow);
 analogWrite(IN4,0);
 delay(1000);
 
 analogWrite(IN3,0);
 analogWrite(IN4,slow);
 delay(1000);
 
 analogWrite(IN3,medium);
 analogWrite(IN4,0);
 delay(1000);
 
analogWrite(IN3,0);
analogWrite(IN4,medium);
delay(1000);
analogWrite(IN3,fast);
analogWrite(IN4,0);
delay(1000);
analogWrite(IN3,0);
analogWrite(IN4,fast);
delay(1000);
 
 analogWrite(IN1, fast);
 analogWrite(IN2, 0);
 
 analogWrite(IN3, 0);
 analogWrite(IN4, fast);
}

