#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
char code[12];
int val = 0;
int bytesread = 0;

SoftwareSerial RFID = SoftwareSerial(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Ready");
  RFID.begin(9600);
  Serial.println("RFID Ready");
}

void loop() {
  val = 0;
  bytesread = 0;
  while(bytesread<12)
  {
    val = RFID.read();
    //Serial.println(val);
   //delay (1000);
        if (val == 3)
    {
      break;
    }
    
    if (val != 2 & val != -1)
    {
      code[bytesread] = val;
      bytesread++;
      
      code[bytesread]=-1;
    }
  }
  
  if (bytesread >= 12)
  {
    Serial.print("Tag: [");
    for (int i=0; code[i]!=-1; i++)
    {
      Serial.print(code[i]);
    }
    Serial.println("]");
  }
}

