//https://rootsaid.com/object-tracking-robot-arduino/
//https://www.youtube.com/watch?v=i2zJUCuzcAk

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11);
int ledPin = 13;
void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    pinMode (2, OUTPUT);
    pinMode (3, OUTPUT);
    pinMode (4, OUTPUT);
    pinMode (7, OUTPUT);
    pinMode (5, OUTPUT);
    pinMode (6, OUTPUT);
    pinMode(ledPin, OUTPUT); 

    analogWrite(5, 180); //motor1 enable pin
    analogWrite(6, 180); //motor2 enable pin

    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the "Protocol Type" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() 
{
    if (!huskylens.request()) 
Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));

    else if(!huskylens.isLearned()) 
Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));

    else if(!huskylens.available()) 
Serial.println(F("No block or arrow appears on the screen!"));

    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
            driveBot(result);
        }    
    }
}


void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}

void driveBot(HUSKYLENSResult result)
{
  if(result.xCenter<=140)
  {
    left();
  }

  else if(result.xCenter>=200)
  {
    right();
  }

    else if((result.xCenter>=140)&&(result.xCenter<=200))
  {
    if(result.width<=20)
    {
      forward();
    }

    else if(result.width>20)
    {
      stop();
    }
  }
  
}


void stop()
{
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(7, LOW);
digitalWrite(4, LOW);
digitalWrite(ledPin, LOW);
Serial.println("Stop");
}
void right()
{
digitalWrite(2, HIGH);
digitalWrite(3, LOW);
digitalWrite(7, LOW);
digitalWrite(4, HIGH);
digitalWrite(ledPin, HIGH);
Serial.println(" Rotate Right");
}
void left()
{
digitalWrite(2, LOW);
digitalWrite(3, HIGH);
digitalWrite(7, HIGH);
digitalWrite(4, LOW);
digitalWrite(ledPin, HIGH);
Serial.println(" Rotate Left");
}

void forward()
{
digitalWrite(2, LOW);
digitalWrite(3, HIGH);
digitalWrite(7, LOW);
digitalWrite(4, HIGH);
digitalWrite(ledPin, HIGH);
Serial.println("Forward");
}

void backward()
{
digitalWrite(2, HIGH);
digitalWrite(3, LOW);
digitalWrite(7, HIGH);
digitalWrite(4, LOW);
digitalWrite(ledPin, HIGH);
Serial.println("Forward");
}
