//const int COL1 = 3;     // Column #1 control
const int LED = 8;     // 'row 1' led
const int Led2 = 12;

void setup() {  
  Serial.begin(9600);
  
  Serial.println("microbit is ready!");

  // because the LEDs are multiplexed, we must ground the opposite side of the LED
//  pinMode(COL1, OUTPUT);
//  digitalWrite(COL1, LOW); 
   
  pinMode(LED, OUTPUT);   
  pinMode(Led2, OUTPUT);
}

void loop(){
  Serial.println("blink!");
  
  digitalWrite(LED, HIGH);
  digitalWrite(Led2, LOW);
  delay(500);
  digitalWrite(LED, LOW);
  digitalWrite(Led2, HIGH);
  delay(500);
}
