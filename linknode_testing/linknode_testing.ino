/*Turn on and off the S3 relay in every second */
 
//12 = D10
//13 = D8
//14 = D4
//16 = D3
 
 void setup()
 {
  
  // pinMode(12,OUTPUT); 
   //pinMode(13, OUTPUT);
   //pinMode(14, OUTPUT);
   pinMode(16, OUTPUT);
   Serial.begin(9600); 
 }
 void loop()
 {
  
   //digitalWrite(12,HIGH);
   //digitalWrite(13, LOW);
   //digitalWrite(14, HIGH);
   digitalWrite(16, LOW);
   Serial.println("Relay ON\n");
   delay(1000);
  
   //digitalWrite(12,LOW);
   //digitalWrite(13, HIGH);
   //digitalWrite(14, LOW);
   digitalWrite(16, HIGH);
   Serial.println("Relay OFF\n");
   delay(1000);  
 }
