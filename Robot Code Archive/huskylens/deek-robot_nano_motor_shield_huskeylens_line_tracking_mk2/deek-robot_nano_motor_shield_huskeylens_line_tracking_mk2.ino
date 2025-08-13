#include "HUKSYLENS.h"
#include "PIDLoop.h"

#define dirA 12
#define pwmA 3 
#define dirB 13 
#define pwmB 11
#define brkA 9 
#define brkB 8 

#define ZUMO_FAST     150

//PIDLoop headingLoop(2700, 150, 0, false); //2300
PIDLoop headingLoop(100, 2.4, 0.0, false); // 50 //2850
HUKSYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
int ID1 = 1;
int left = 0, right = 0;

void setup() {
  Serial.begin(115200);
  pinMode(dirA, OUTPUT);
pinMode(dirB, OUTPUT);  
pinMode(pwmA, OUTPUT);
pinMode(pwmB, OUTPUT);
  pinMode(brkA, OUTPUT);
pinMode(brkB, OUTPUT);
digitalWrite(brkA, LOW);
digitalWrite(brkB, LOW);
Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUKSYLENS (General Settings>>Protol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
}

void loop() {
 int32_t error; 
    if (!huskylens.request(ID1)) {Serial.println(F("Fail to request objects from HUSKYLENS!"));left = 0; right = 0;}
    else if(!huskylens.isLearned()) {Serial.println(F("Object not learned!"));left = 0; right = 0;}
    else if(!huskylens.arrows.available()) {Serial.println(F("Object disappeared!")); left = 0; right = 0;}
    else
    {
        HUSKYLENSResult result = huskylens.arrows.read(ID1);
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);

        // Calculate heading error with respect to m_x1, which is the far-end of the vector,
        // the part of the vector we're heading toward.
        error = (int32_t)result.xTarget - (int32_t)160;

        // pixy.line.vectors->print();
        // Perform PID calcs on heading error.
        headingLoop.update(error);

        // separate heading into left and right wheel velocities.
        left = -headingLoop.m_command;
        right = headingLoop.m_command;

        left += ZUMO_FAST;
        right += ZUMO_FAST;
    }

    Serial.println(String()+left+","+right);
    forward(left, right);
//delay(1000);
}

void stop() {
  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);
  digitalWrite(dirA, 0);
  digitalWrite(dirB, 0);
}

void forward(int lspd, int rspd) {
  //analogWrite(pwmA, 255 - lspd);
  analogWrite(pwmA, abs(lspd));
  digitalWrite(dirA, HIGH);
  //analogWrite(pwmB, 255 - rspd);
  analogWrite(pwmB, abs(rspd));
  digitalWrite(dirB, HIGH);
}

void back(int lspd, int rspd) {
  analogWrite(pwmA, lspd);
  digitalWrite(dirA, LOW);
  analogWrite(pwmB, rspd);
  digitalWrite(dirB, LOW);
}
