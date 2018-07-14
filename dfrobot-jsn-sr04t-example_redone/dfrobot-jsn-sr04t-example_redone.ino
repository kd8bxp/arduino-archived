//https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207

#define ECHOPIN 2// Pin to receive echo pulse
#define TRIGPIN 3// Pin to send trigger pulse

int distance, count, sum;

void setup(){
  Serial.begin(9600);
  pinMode(ECHOPIN, INPUT_PULLUP);
  pinMode(TRIGPIN, OUTPUT);
  //digitalWrite(ECHOPIN, HIGH);
}

void loop(){
  ping();
  delay(150);
  
}
void ping(){
  int avg = 50;
  count = count + 1;
  for (int i=0;i<avg;i++){
  digitalWrite(TRIGPIN, LOW); // Set the trigger pin to low for 2uS
  delayMicroseconds(6);
  digitalWrite(TRIGPIN, HIGH); // Send a 10uS high to trigger ranging
  delayMicroseconds(12);
  digitalWrite(TRIGPIN, LOW); // Send pin low again
  //digitalWrite(ECHOPIN, LOW);
  int duration = pulseIn(ECHOPIN,HIGH); //,38000); // Read in times pulse
  duration = duration/148;
  if (duration > 100 || duration < 0) {count = count -1; } else {
  sum = sum + duration; }
  }
  distance = sum / avg ; //count;
  Serial.print(distance);
  Serial.println("   in");
  sum = 0;
  if (count >= avg) {count = 0;}                    
 // delay(150);// Wait 50mS before next ranging
}

