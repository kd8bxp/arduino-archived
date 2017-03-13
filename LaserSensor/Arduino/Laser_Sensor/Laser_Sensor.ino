int laser_din=2;

void setup()
{
  pinMode(laser_din,INPUT);
  Serial.begin(9600);
}
void loop()
{
  if(digitalRead(laser_din)==LOW)
  {
    Serial.println("NO Obstacles!");
  }
  else
  {
    Serial.println("Obstacles!");
  }
  delay(500);
}
