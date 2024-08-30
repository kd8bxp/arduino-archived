//view  <robonii.h> for custom board defines

/*
  This code will blink an Robonii LED as a heartbeat.
  Every 2 seconds a reading from the battery is taken, converted and sent via the USB port.
  An analogue value is written to both obstacle detection LED.s This will light up an LED at 2 diffrent levels...
 */
int inByte = 0;         // incoming serial byte


void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(D3_Green, OUTPUT);     
  pinMode(D2_Green, OUTPUT);  
    pinMode(Obstacle_Left, OUTPUT);  
    pinMode(Obstacle_Right, OUTPUT); 

   analogWrite(Obstacle_Left, 100);
  analogWrite(Obstacle_Right, 100);
  

    digitalWrite(D1_Green, LOW);   // set the LED on
   Serial.begin(115200);   
   Serial.println("  ");  
   Serial.println("  ");  
Serial.println("-----------  Serial Started at 115200 BAUD: -----------");  
   
  
}

unsigned char Brightness = 0;

void loop() {
 
  Brightness = Brightness + 20;
  digitalWrite(D2_Green, LOW);    // set the LED on
  digitalWrite(D1_Green, LOW);    // set the LED off
  
  analogWrite(Obstacle_Right, Brightness);
  analogWrite(Obstacle_Left, Brightness);
  
  delay(1000);       
  Brightness = Brightness + 20;
   digitalWrite(D2_Green, HIGH);    // set the LED on
  digitalWrite(D1_Green, HIGH);    // set the LED off
  
  analogWrite(Obstacle_Right, Brightness);
  analogWrite(Obstacle_Left, Brightness);
      
 unsigned int batVolt = analogRead(VBat_Pin);
  double Voltage = batVolt * 0.0009765 ; //Default Reference is intrnal 1V ref. Thus each bit = 1V/1024 (12bit) = 0.0009765V/bit.  Could use define "A2D_12Bit_1VRef_bit_Value"
  double ScaleVoltage = Voltage / VBat_Resistor_Scale; // Could use 0.10569 = 3.9K/(33K+3.9K). As this is the Resistor scaleing factor.   
   
   Serial.println(" ===============================================  ");
     Serial.print("Analog Output Brightness: ");
  Serial.println(Brightness);
  Serial.println(" ===============================================  ");
   Serial.print("Analog Reading: ");
  Serial.println(batVolt);
  Serial.println(" ===============================================  ");
  Serial.print("Reading Voltage : ");
  Serial.println(Voltage);
  Serial.println(" ===============================================  ");
    Serial.print("Scalled to R-network Voltage Reading: ");
  Serial.print(ScaleVoltage);
  Serial.println("V");
  Serial.println(" ===============================================  ");
  delay(1000);   
}




