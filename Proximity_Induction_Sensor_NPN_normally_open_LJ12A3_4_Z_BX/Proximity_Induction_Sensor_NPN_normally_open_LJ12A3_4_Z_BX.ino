/*
TEST CODE FOR PROXIMITY SENSOR 
Metal Detection with 3 wire sensor
*/

float metalDetected;
int monitoring;
int metalDetection = 1;

void setup(){
  Serial.begin(9600);
}

void loop(){
  monitoring = analogRead(metalDetection);
  metalDetected = (float) monitoring*100/1024.0;
  Serial.print("14CORE METAL DETECTOR TEST");
  delay(500);
  Serial.print("Initializing Proximity Sensor");
  delay(500);
  Serial.print("Please wait...");
  delay(1000);
  Serial.print("Metal is Proximited = ");
  Serial.print(metalDetected);
  Serial.println("%");
  if (monitoring > 250)
    Serial.println("Metal is Detected");
  delay(1000);
}
