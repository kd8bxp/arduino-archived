/* ==================================================================================================================================================
         Project: MT8870 DTMF Servo sketch
          Author: Scott C
         Created: 4th August 2015
     Arduino IDE: 1.6.4
         Website: http://arduinobasics.blogspot.com/p/arduino-basics-projects-page.html
     Description: This project will allow you to control a Servo motor using an Arduino UNO and a MT8870 DTMF Module.
                  The DTMF signal is received through the 3.5mm port of the DTMF module and is decoded. We will use the
                  decoded output to control the position of the Servo. A SG-5010 Servo motor was used in this project.
===================================================================================================================================================== */
//This sketch uses the Servo library that comes with the Arduino IDE

//Global variables-----------------------------------------------------------------------------------------
 
byte DTMFread;            // The DTMFread variable will be used to interpret the output of the DTMF module.
const int STQ = 3;        // Attach DTMF Module STQ Pin to Arduino Digital Pin 3
 const int Q4 = 4;        // Attach DTMF Module Q4  Pin to Arduino Digital Pin 4
 const int Q3 = 5;        // Attach DTMF Module Q3  Pin to Arduino Digital Pin 5
 const int Q2 = 6;        // Attach DTMF Module Q2  Pin to Arduino Digital Pin 6
 const int Q1 = 7;        // Attach DTMF Module Q1  Pin to Arduino Digital Pin 7


/*=========================================================================================================
setup() : will setup the Servo, and prepare the Arduino to receive the MT8700 DTMF module's output.
========================================================================================================== */
void setup() {
  
  Serial.begin(9600);
  //Setup the INPUT pins on the Arduino
  pinMode(STQ, INPUT);
  pinMode(Q4, INPUT);
  pinMode(Q3, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q1, INPUT);
}

/*=========================================================================================================
loop() : Arduino will interpret the DTMF module output and position the Servo accordingly
========================================================================================================== */
void loop() {
  if(digitalRead(STQ)==HIGH){       //When a DTMF tone is detected, STQ will read HIGH for the duration of the tone.
    DTMFread=0;
    if(digitalRead(Q1)==HIGH){      //If Q1 reads HIGH, then add 1 to the DTMFread variable
      DTMFread=DTMFread+1;
    }
    if(digitalRead(Q2)==HIGH){      //If Q2 reads HIGH, then add 2 to the DTMFread variable
      DTMFread=DTMFread+2;
    }
    if(digitalRead(Q3)==HIGH){      //If Q3 reads HIGH, then add 4 to the DTMFread variable
      DTMFread=DTMFread+4;
    }
    if(digitalRead(Q4)==HIGH){      //If Q4 reads HIGH, then add 8 to the DTMFread variable
      DTMFread=DTMFread+8;
    }
    Serial.println(DTMFread);
      }
  
}