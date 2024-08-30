
#include <Driver_RFID.h>
//This code reads a Robonii-RFID tag or game-objects and prints the ID on the serial port

void setup() {
  // put your setup code here, to run once:


pinMode(D1_Green, OUTPUT);  
pinMode(D2_Green, OUTPUT);  
pinMode(D3_Green, OUTPUT);  
pinMode(D4_Green, OUTPUT);  
pinMode(D5_Green, OUTPUT);  
pinMode(D1_Red, OUTPUT);  
pinMode(D2_Red, OUTPUT);  
pinMode(D3_Red, OUTPUT); 


digitalWrite(D1_Green, HIGH);
digitalWrite(D2_Green, HIGH);
digitalWrite(D3_Green, HIGH);
digitalWrite(D4_Green, HIGH);
digitalWrite(D5_Green, HIGH);
digitalWrite(D1_Red, HIGH);
digitalWrite(D2_Red, HIGH);
digitalWrite(D3_Red, HIGH);

RFID.rfid_init();

//Enable RFID
e_rfid_state ee = RFID_ENABLED_ON;
RFID.rfid_set_state(ee);

Serial.print("RDIF Setup Done: ");

}

void loop() {
  // put your main code here, to run repeatedly: 
  delay(2000);

  if (RFID.rfid_flag_tag_data_updated)
  {
    //Tag Read - Print tag ID to USB
    unsigned char rfid_TAG[10];
    RFID.GetRFID(rfid_TAG);
    Serial.print("TAG Read: ");
    Serial.print(rfid_TAG[0]);
    Serial.print(rfid_TAG[1]);
    Serial.print(rfid_TAG[2]);
    Serial.print(rfid_TAG[3]);
    Serial.println(rfid_TAG[4]);    
  }
}


